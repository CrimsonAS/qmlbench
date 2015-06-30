#include <iostream>

#include <QtCore>
#include <QtGui>
#include <QtQuick>

#include "qcommandlineparser.h"

static bool onlyPrintJson = false;

class ResultRecorder
{
    static QVariantMap m_results;

public:
    static void startResults(const QString &id)
    {
        m_results["id"] = id;

        QVariantMap osMap;
        osMap["prettyProductName"] = QSysInfo::prettyProductName();
        osMap["platformPlugin"] = QGuiApplication::platformName();
        m_results["os"] = osMap;

        // The following code makes the assumption that an OpenGL context the GUI
        // thread will get the same capabilities as the render thread's OpenGL
        // context. Not 100% accurate, but it works...
        QOpenGLContext context;
        context.create();
        QOffscreenSurface surface;
        // In very odd cases, we can get incompatible configs here unless we pass the
        // GL context's format on to the offscreen format.
        surface.setFormat(context.format());
        surface.create();
        if (!context.makeCurrent(&surface)) {
            qWarning() << "failed to acquire GL context to get version info.";
            return;
        }

        QOpenGLFunctions *func = context.functions();
#if QT_VERSION >= 0x050300
        const char *vendor = (const char *) func->glGetString(GL_VENDOR);
        const char *renderer = (const char *) func->glGetString(GL_RENDERER);
        const char *version = (const char *) func->glGetString(GL_VERSION);
#else
        Q_UNUSED(func);
        const char *vendor = (const char *) glGetString(GL_VENDOR);
        const char *renderer = (const char *) glGetString(GL_RENDERER);
        const char *version = (const char *) glGetString(GL_VERSION);
#endif

        if (!onlyPrintJson) {
            std::cout << "ID:          " << id.toStdString() << std::endl;
            std::cout << "OS:          " << QSysInfo::prettyProductName().toStdString() << std::endl;
            std::cout << "QPA:         " << QGuiApplication::platformName().toStdString() << std::endl;
            std::cout << "GL_VENDOR:   " << vendor << std::endl;
            std::cout << "GL_RENDERER: " << renderer << std::endl;
            std::cout << "GL_VERSION:  " << version << std::endl;
        }

        QVariantMap glInfo;
        glInfo["vendor"] = vendor;
        glInfo["renderer"] = renderer;
        glInfo["version"] = version;

        m_results["opengl"] = glInfo;

        context.doneCurrent();
    }

    static void recordWindowSize(const QSize &windowSize)
    {
        m_results["windowSize"] = QString::number(windowSize.width()) + "x" + QString::number(windowSize.height());
    }

    static void recordOperationsPerFrame(const QString &benchmark, int ops)
    {
        QVariantMap benchMap = m_results[benchmark].toMap();
        QVariantList benchResults = benchMap["results"].toList();
        benchResults.append(ops);

        benchMap["results"] = benchResults;
        m_results[benchmark] = benchMap;

        if (!onlyPrintJson)
            std::cout << "    " << ops << " ops/frame" << std::endl;
    }

    static void recordOperationsPerFrameAverage(const QString &benchmark, int ops)
    {
        QVariantMap benchMap = m_results[benchmark].toMap();
        benchMap["average"] = ops;
        m_results[benchmark] = benchMap;

        if (!onlyPrintJson)
            std::cout << "    " << ops << " ops/frame average" << std::endl;
    }

    static void finish()
    {
        if (onlyPrintJson) {
            QJsonDocument results = QJsonDocument::fromVariant(m_results);
            std::cout << results.toJson().toStdString();
        }
        m_results.clear();
    }
};
QVariantMap ResultRecorder::m_results;

class FpsDecider : public QWindow
{
public:
    FpsDecider()
        : gl(0)
        , frameCount(0)
    {
        setSurfaceType(OpenGLSurface);
        QSurfaceFormat format;
#if QT_VERSION >= 0x050300
        format.setSwapInterval(1);
#endif
        setFormat(format);
    }

    void exposeEvent(QExposeEvent *) {
        if (isExposed())
            render();
    }

    bool event(QEvent *e)
    {
        if (e->type() == QEvent::UpdateRequest) {
            render();
            return true;
        }
        return QWindow::event(e);
    }

    void render()
    {
        if (!gl) {
            gl = new QOpenGLContext();
            gl->setFormat(format());
            gl->create();
            timer.start();
        }

        gl->makeCurrent(this);
        QOpenGLFunctions *func = gl->functions();

        if (frameCount == 0) {
#if QT_VERSION >= 0x050300
            std::cout << "GL_VENDOR:   " << (const char *) func->glGetString(GL_VENDOR) << std::endl;
            std::cout << "GL_RENDERER: " << (const char *) func->glGetString(GL_RENDERER) << std::endl;
            std::cout << "GL_VERSION:  " << (const char *) func->glGetString(GL_VERSION) << std::endl;
#else
            Q_UNUSED(func);
            std::cout << "GL_VENDOR:   " << (const char *) glGetString(GL_VENDOR) << std::endl;
            std::cout << "GL_RENDERER: " << (const char *) glGetString(GL_RENDERER) << std::endl;
            std::cout << "GL_VERSION:  " << (const char *) glGetString(GL_VERSION) << std::endl;
#endif
        }

        ++frameCount;
        int c = frameCount % 2;

#if QT_VERSION >= 0x050300
        func->glClearColor(c, 0, 1 - c, 1);
        func->glClear(GL_COLOR_BUFFER_BIT);
#else
        glClearColor(c, 0, 1 - c, 1);
        glClear(GL_COLOR_BUFFER_BIT);
#endif

        gl->swapBuffers(this);

        int time = timer.elapsed();
        if (time > 5000) {
            std::cout << std::endl
                     << "FPS: " << frameCount * 1000 / float(time)
                     << " -- " << frameCount << " frames in " << time << "ms; "
                     << time / float(frameCount) << " ms/frame " << std::endl;
            exit(0);

        } else {
            QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
        }
    }

private:
    QOpenGLContext *gl;
    QElapsedTimer timer;
    int frameCount;
};



struct Options
{
    Options()
        : fullscreen(false)
        , verbose(false)
        , repeat(1)
        , delayedStart(0)
        , count(-1)
        , fpsTolerance(0.05)
        , fpsInterval(1000)
        , fpsOverride(0)
        , windowSize(800, 600)
    {
    }

    QString bmTemplate;
    bool fullscreen;
    bool verbose;
    int repeat;
    int delayedStart;
    int count;
    qreal fpsTolerance;
    qreal fpsInterval;
    qreal fpsOverride;
    qreal targetFps;
    QSize windowSize;
};



struct Benchmark
{
    Benchmark(const QString &file)
        : fileName(file)
        , completed(false)
    {
    }

    QString fileName;
    QSize windowSize;

    bool completed;
    QList<qreal> operationsPerFrame;
};



class BenchmarkRunner : public QObject
{
    Q_OBJECT

    // None of these are strictly constant, but for the sake of one QML run, they are
    // so flag it for simplicity
    Q_PROPERTY(QQuickView *view READ view CONSTANT)
    Q_PROPERTY(QQmlComponent *component READ component CONSTANT)
    Q_PROPERTY(qreal screeRefreshRate READ screenRefreshRate CONSTANT)
    Q_PROPERTY(QString input READ input CONSTANT)
    Q_PROPERTY(qreal fpsTolerance READ fpsTolerance CONSTANT)
    Q_PROPERTY(qreal fpsInterval READ fpsInterval CONSTANT)
    Q_PROPERTY(bool verbose READ verbose CONSTANT)
    Q_PROPERTY(int count READ count CONSTANT)

public:
    BenchmarkRunner();
    ~BenchmarkRunner();

    bool execute();

    QList<Benchmark> benchmarks;
    Options options;

    QQuickView *view() const { return m_view; }
    QQmlComponent *component() const { return m_component; }
    qreal screenRefreshRate() const { return options.fpsOverride > 0 ? options.fpsOverride : m_view->screen()->refreshRate(); }
    QString input() const { return benchmarks[m_currentBenchmark].fileName; }

    qreal fpsTolerance() const { return options.fpsTolerance / 100.0; }
    qreal fpsInterval() const { return options.fpsInterval; }

    bool verbose() const { return options.verbose; }

    int count() const { return options.count; }

public slots:
    void recordOperationsPerFrame(qreal count);
    void complete();
    void abort();

private slots:
    void start();
    void maybeStartNext();

private:
    void abortAll();

    int m_currentBenchmark;

    QQuickView *m_view;
    QQmlComponent *m_component;
};



int main(int argc, char **argv)
{
    qmlRegisterType<QQuickView>();

    QGuiApplication app(argc, argv);

	QCommandLineParser parser;

    QCommandLineOption decideFpsOption(QStringLiteral("decide-fps"), QStringLiteral("Run a simple test to decide the frame rate of the primary screen"));
    parser.addOption(decideFpsOption);

    QCommandLineOption verboseOption(QStringList() << QStringLiteral("v") << QStringLiteral("verbose"),
                                     QStringLiteral("Verbose mode"));
    parser.addOption(verboseOption);

    QCommandLineOption idOption(QStringLiteral("id"),
                                QStringLiteral("Provides a unique identifier for this run in the JSON output."),
                                QStringLiteral("identifier"),
                                QStringLiteral(""));
    parser.addOption(idOption);

    QCommandLineOption jsonOption(QStringLiteral("json"),
                                QStringLiteral("Switches to provide JSON output of benchmark runs."));
    parser.addOption(jsonOption);

    QCommandLineOption repeatOption(QStringLiteral("repeat"),
                                         QStringLiteral("Sets the number of times to repeat the benchmark, to get more stable results"),
                                         QStringLiteral("iterations"),
                                         QStringLiteral("5"));
    parser.addOption(repeatOption);

    QCommandLineOption delayOption(QStringLiteral("delay"),
                                   QStringLiteral("Initial delay before benchmarks start"),
                                   QStringLiteral("milliseconds"),
                                   QStringLiteral("2000"));
    parser.addOption(delayOption);

    QCommandLineOption widthOption(QStringLiteral("width"),
                                   QStringLiteral("Window Width"),
                                   QStringLiteral("width"),
                                   QStringLiteral("800"));
    parser.addOption(widthOption);

    QCommandLineOption heightOption(QStringLiteral("height"),
                                   QStringLiteral("Window height"),
                                   QStringLiteral("height"),
                                   QStringLiteral("600"));
    parser.addOption(heightOption);

    QCommandLineOption fpsIntervalOption(QStringLiteral("fps-interval"),
                                         QStringLiteral("Set the interval used to measure framerate in ms. Higher values lead to more stable test results"),
                                         QStringLiteral("interval"),
                                         QStringLiteral("1000"));
    parser.addOption(fpsIntervalOption);

    QCommandLineOption fpsToleranceOption(QStringLiteral("fps-tolerance"),
                                          QStringLiteral("The amount of deviance tolerated from the target frame rate in %. Lower value leads to more accurate results"),
                                          QStringLiteral("tolerance"),
                                          QStringLiteral("2"));
    parser.addOption(fpsToleranceOption);

    QCommandLineOption fpsOverrideOption(QStringLiteral("fps-override"),
                                         QStringLiteral("Override QScreen::refreshRate() with a custom refreshrate"),
                                         QStringLiteral("framerate"));
    parser.addOption(fpsOverrideOption);

    QCommandLineOption fullscreenOption(QStringLiteral("fullscreen"), QStringLiteral("Run graphics in fullscreen mode"));
    parser.addOption(fullscreenOption);

    QCommandLineOption templateOption(QStringList() << QStringLiteral("s") << QStringLiteral("shell"),
                                      QStringLiteral("What kind of benchmark shell to run: 'sustained-fps', 'static-count"),
                                      QStringLiteral("template"));
    parser.addOption(templateOption);

    QCommandLineOption countOption(QStringLiteral("count"),
                                   QStringLiteral("Static option for use with 'static-count' shell"),
                                   QStringLiteral("count"),
                                   QStringLiteral("-1"));
    parser.addOption(countOption);


    parser.addPositionalArgument(QStringLiteral("input"),
                                 QStringLiteral("One or more QML files or a directory of QML files to benchmark"));
    const QCommandLineOption &helpOption = parser.addHelpOption();

    parser.process(app);

    if (parser.isSet(jsonOption)) {
        onlyPrintJson = true;
    }

    if (parser.isSet(decideFpsOption)) {
        FpsDecider fpsDecider;
        if (parser.isSet(fullscreenOption))
            fpsDecider.showFullScreen();
        else
            fpsDecider.show();
        fpsDecider.raise();
        return app.exec();
    }

    if (parser.isSet(helpOption) || parser.positionalArguments().size() == 0) {
        parser.showHelp(0);
    }

    BenchmarkRunner runner;
    runner.options.verbose = parser.isSet(verboseOption);
    runner.options.fullscreen = parser.isSet(fullscreenOption);
    runner.options.repeat = qMax<int>(1, parser.value(repeatOption).toInt());
    runner.options.fpsInterval = qMax<qreal>(500, parser.value(fpsIntervalOption).toFloat());
    runner.options.fpsTolerance = qMax<qreal>(1, parser.value(fpsToleranceOption).toFloat());
    runner.options.bmTemplate = parser.value(templateOption);
    runner.options.delayedStart = parser.value(delayOption).toInt();
    runner.options.count = parser.value(countOption).toInt();

    QSize size(parser.value(widthOption).toInt(),
               parser.value(heightOption).toInt());

    if (size.isValid())
        runner.options.windowSize = size;

    ResultRecorder::startResults(parser.value(idOption));
    ResultRecorder::recordWindowSize(runner.options.windowSize);

    if (parser.isSet(fpsOverrideOption))
        runner.options.fpsOverride = parser.value(fpsOverrideOption).toFloat();

    if (runner.options.bmTemplate == QStringLiteral("sustained-fps"))
        runner.options.bmTemplate = QStringLiteral("qrc:/Shell_SustainedFpsWithCount.qml");
    else if (runner.options.bmTemplate == QStringLiteral("static-count"))
        runner.options.bmTemplate = QStringLiteral("qrc:/Shell_SustainedFpsWithStaticCount.qml");
    else
        runner.options.bmTemplate = QStringLiteral("qrc:/Shell_SustainedFpsWithCount.qml");

    foreach (QString input, parser.positionalArguments()) {
        QFileInfo info(input);
        if (!info.exists()) {
            qWarning() << "input doesn't exist:" << input;
        } else if (info.suffix() == QStringLiteral("qml")) {
            runner.benchmarks << Benchmark(info.absoluteFilePath());
        } else if (info.isDir()) {
            QDirIterator iterator(input, QStringList() << QStringLiteral("*.qml"));
            while (iterator.hasNext()) {
                runner.benchmarks << Benchmark(iterator.next());
            }
        }
    }

    if (runner.options.verbose) {
        std::cout << "Frame Rate .........: " << (runner.options.fpsOverride > 0 ? runner.options.fpsOverride : QGuiApplication::primaryScreen()->refreshRate()) << std::endl;
        std::cout << "Fullscreen .........: " << (runner.options.fullscreen ? "yes" : "no") << std::endl;
        std::cout << "Fullscreen .........: " << (runner.options.fullscreen ? "yes" : "no") << std::endl;
        std::cout << "Fps Interval .......: " << runner.options.fpsInterval << std::endl;
        std::cout << "Fps Tolerance ......: " << runner.options.fpsTolerance << std::endl;
        std::cout << "Template ...........: " << runner.options.bmTemplate.toStdString() << std::endl;
        std::cout << "Benchmarks:" << std::endl;
        foreach (const Benchmark &b, runner.benchmarks) {
            std::cout << " - " << b.fileName.toStdString() << std::endl;
        }
    }

    if (!runner.execute())
        return 0;

    int ret = app.exec();
    ResultRecorder::finish();
    return ret;
}

BenchmarkRunner::BenchmarkRunner()
    : m_currentBenchmark(0)
    , m_view(0)
{
}

BenchmarkRunner::~BenchmarkRunner()
{
    delete m_view;
}

bool BenchmarkRunner::execute()
{
    m_currentBenchmark = 0;
    if (benchmarks.size() == 0)
        return false;
    QTimer::singleShot(options.delayedStart, this, SLOT(start()));

    m_view = new QQuickView();
    // Make sure proper fullscreen is possible on OSX
    m_view->setFlags(Qt::Window
                     | Qt::WindowSystemMenuHint
                     | Qt::WindowTitleHint
                     | Qt::WindowMinMaxButtonsHint
                     | Qt::WindowCloseButtonHint
                     | Qt::WindowFullscreenButtonHint);
    m_view->setResizeMode(QQuickView::SizeRootObjectToView);
    m_view->rootContext()->setContextProperty("benchmark", this);

    m_view->resize(options.windowSize);

    if (options.fullscreen)
        m_view->showFullScreen();
    else
        m_view->show();
    m_view->raise();

    return true;
}

void BenchmarkRunner::start()
{
    Benchmark &bm = benchmarks[m_currentBenchmark];

    if (bm.operationsPerFrame.size() == 0 && !onlyPrintJson)
        std::cout << "running: " << bm.fileName.toStdString() << std::endl;

    m_component = new QQmlComponent(m_view->engine(), bm.fileName);
    if (m_component->status() != QQmlComponent::Ready) {
        qWarning() << "component is not ready" << bm.fileName;
        abort();
        return;
    }

    m_view->setSource(QUrl(options.bmTemplate));
    if (!m_view->rootObject()) {
        qWarning() << "no root object..";
        abortAll();
        return;
    }

    bm.windowSize = m_view->size();
}

void BenchmarkRunner::maybeStartNext()
{

    ++m_currentBenchmark;
    if (m_currentBenchmark < benchmarks.size()) {
        QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
    } else {
        if (!onlyPrintJson)
            std::cout << "All done..." << std::endl;
        qApp->quit();
    }
}

void BenchmarkRunner::abort()
{
    maybeStartNext();
}

void BenchmarkRunner::abortAll()
{
    qWarning() << "Aborting all benchmarks...";
    qApp->quit();
}

void BenchmarkRunner::recordOperationsPerFrame(qreal ops)
{
    Benchmark &bm = benchmarks[m_currentBenchmark];
    bm.completed = true;
    bm.operationsPerFrame << ops;
    ResultRecorder::recordOperationsPerFrame(bm.fileName, ops);
    if (bm.operationsPerFrame.size() == options.repeat && options.repeat > 1) {
        qreal avg = 0;
        foreach (qreal r, bm.operationsPerFrame)
            avg += r;
        ResultRecorder::recordOperationsPerFrameAverage(bm.fileName, avg / options.repeat);
    }
    complete();
}

void BenchmarkRunner::complete()
{
    m_component->deleteLater();
    m_component = 0;
    if (benchmarks[m_currentBenchmark].operationsPerFrame.size() < options.repeat)
        QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
    else
        QMetaObject::invokeMethod(this, "maybeStartNext", Qt::QueuedConnection);
}

#include "main.moc"
