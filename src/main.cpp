/****************************************************************************
**
** Copyright (C) 2017 Crimson AS <info@crimson.no>
** Contact: https://www.qt.io/licensing/
**
** This file is part of the qmlbench tool.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <iostream>
#include <iomanip>

#include <QtCore>
#include <QtGui>
#include <QtQuick>

#include "options.h"
#include "qcommandlineparser.h"

Options Options::instance;

class ResultRecorder
{
    static QVariantMap m_results;

public:
    static void startResults(const QString &id)
    {
        if (Options::instance.isSubProcess)
            return; // parent process will get all this.

        m_results["id"] = id;

        QString prettyProductName =
#if QT_VERSION >= 0x050400
            QSysInfo::prettyProductName();
#else
#  if defined(Q_OS_IOS)
            QStringLiteral("iOS");
#  elif defined(Q_OS_OSX)
            QString::fromLatin1("OSX %d").arg(QSysInfo::macVersion());
#  elif defined(Q_OS_WIN)
            QString::fromLatin1("Windows %d").arg(QSysInfo::windowsVersion());
#  elif defined(Q_OS_LINUX)
            QStringLiteral("Linux");
#  elif defined(Q_OS_ANDROID)
            QStringLiteral("Android");
#  else
            QStringLiteral("unknown");
#  endif
#endif

        QVariantMap osMap;
        osMap["prettyProductName"] = prettyProductName;
        osMap["platformPlugin"] = QGuiApplication::platformName();
        m_results["os"] = osMap;
        m_results["qt"] = QT_VERSION_STR;
        m_results["command-line"] = qApp->arguments().join(' ');

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

        if (!Options::instance.onlyPrintJson) {
            std::cout << "ID:          " << id.toStdString() << std::endl;
            std::cout << "OS:          " << prettyProductName.toStdString() << std::endl;
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

        if (!Options::instance.onlyPrintJson) {
            if (opsAreActuallyFrames)
                std::cout << "    " << ops << " frames" << std::endl;
            else
                std::cout << "    " << ops << " ops/frame" << std::endl;
        }
    }

    static void recordOperationsPerFrameAverage(const QString &benchmark, qreal ops, int samples, qreal stddev, qreal median)
    {
        QVariantMap benchMap = m_results[benchmark].toMap();
        benchMap["average"] = ops;
        benchMap["samples-in-average"] = samples;
        benchMap["samples-total"] = samples; // compatibility
        benchMap["standard-deviation"] = stddev;
        benchMap["standard-deviation-all-samples"] = stddev; // compatibility
        benchMap["standard-error"] = stddev / sqrt(samples);
        benchMap["coefficient-of-variation"] = stddev / ops;
        benchMap["median"] = median;

        if (!Options::instance.onlyPrintJson) {
            std::string opsString;
            if (opsAreActuallyFrames)
                opsString = " frames";
            else
                opsString = " ops/frame";

            std::cout << "    Average: " << ops << " " << opsString << ";"
                      << " using " << " samples"
                      << "; MedianAll=" << median
                      << "; StdDev=" << stddev
                      << ", CoV=" << (stddev / ops)
                      << std::endl;
        }

        m_results[benchmark] = benchMap;
    }

    static void mergeResults(const QString &benchmark, const QJsonObject &o)
    {
        m_results[benchmark] = o.toVariantMap();
    }

    static void finish()
    {
        if (Options::instance.onlyPrintJson) {
            QJsonDocument results = QJsonDocument::fromVariant(m_results);
            std::cout << results.toJson().constData();
        }
        m_results.clear();
    }

    static bool opsAreActuallyFrames;
};
QVariantMap ResultRecorder::m_results;
bool ResultRecorder::opsAreActuallyFrames = false;

struct Benchmark
{
    Benchmark(const QString &file)
        : fileName(file)
    {
    }

    QString fileName;
    QSize windowSize;

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
    Q_PROPERTY(double hardwareMultiplier READ hardwareMultiplier CONSTANT)
    Q_PROPERTY(int frameCountInterval READ frameCountInterval CONSTANT)

public:
    BenchmarkRunner();
    ~BenchmarkRunner();

    bool execute();

    QList<Benchmark> benchmarks;

    QQuickView *view() const { return m_view; }
    QQmlComponent *component() const { return m_component; }
    qreal screenRefreshRate() const { return Options::instance.fpsOverride > 0 ? Options::instance.fpsOverride : m_view->screen()->refreshRate(); }
    QString input() const { return benchmarks[m_currentBenchmark].fileName; }

    qreal fpsTolerance() const { return Options::instance.fpsTolerance / 100.0; }
    qreal fpsInterval() const { return Options::instance.fpsInterval; }

    int frameCountInterval() const { return Options::instance.frameCountInterval; }

    bool verbose() const { return Options::instance.verbose; }

    int count() const { return Options::instance.count; }

    double hardwareMultiplier() const { return Options::instance.hardwareMultiplier; }

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

QStringList processCommandLineArguments(const QGuiApplication &app, BenchmarkRunner &runner)
{
    QCommandLineParser parser;

    QCommandLineOption subprocessOption("silently-really-run-and-bypass-subprocess");

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
    subprocessOption.setFlags(subprocessOption.flags() | QCommandLineOption::HiddenFromHelp);
#else
    subprocessOption.setHidden(true);
#endif

    parser.addOption(subprocessOption);

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
                                      QStringLiteral("What kind of benchmark shell to run: 'sustained-fps', 'static-count', 'frame-count'"),
                                      QStringLiteral("template"));
    parser.addOption(templateOption);

    QCommandLineOption countOption(QStringLiteral("count"),
                                   QStringLiteral("Defines how many instances to create for use with 'static-count' or 'frame-count' shell. Overrides the benchmark's 'count' and 'staticCount' properties."),
                                   QStringLiteral("count"),
                                   QStringLiteral("-1"));
    parser.addOption(countOption);

    QCommandLineOption frameCountInterval(QStringLiteral("framecount-interval"),
                                          QStringLiteral("Sets the interval used to count frames in milliseconds. Only applicable to 'frame-count' shell."),
                                          QStringLiteral("count"),
                                          QStringLiteral("20000"));
    parser.addOption(frameCountInterval);

    QCommandLineOption hardwareMultiplierOption(QStringLiteral("hardware-multiplier"),
                                   QStringLiteral("Defines a multiplier to apply to the 'staticCount' options of benchmarks, so that faster (or slower) hardware can be compared with minimal modifications to benchmarks. For use with 'static-count' or 'frame-count' shell"),
                                   QStringLiteral("hw-mul"),
                                   QStringLiteral("1.0"));
    parser.addOption(hardwareMultiplierOption);

    parser.addPositionalArgument(QStringLiteral("input"),
                                 QStringLiteral("One or more QML files or a directory of QML files to benchmark"));
    const QCommandLineOption &helpOption = parser.addHelpOption();

    parser.process(app);

    Options::instance.isSubProcess = parser.isSet(subprocessOption);

    if (parser.isSet(jsonOption)) {
        Options::instance.onlyPrintJson = true;
    }

    if (parser.isSet(helpOption) || parser.positionalArguments().size() == 0) {
        parser.showHelp(0);
    }

    Options::instance.id = parser.value(idOption);
    Options::instance.verbose = parser.isSet(verboseOption);
    Options::instance.fullscreen = parser.isSet(fullscreenOption);
    Options::instance.repeat = qMax<int>(1, parser.value(repeatOption).toInt());
    Options::instance.fpsInterval = qMax<qreal>(500, parser.value(fpsIntervalOption).toFloat());
    Options::instance.fpsTolerance = qMax<qreal>(1, parser.value(fpsToleranceOption).toFloat());
    Options::instance.bmTemplate = parser.value(templateOption);
    Options::instance.delayedStart = parser.value(delayOption).toInt();
    Options::instance.count = parser.value(countOption).toInt();
    Options::instance.hardwareMultiplier = parser.value(hardwareMultiplierOption).toDouble();
    Options::instance.frameCountInterval = parser.value(frameCountInterval).toInt();

    QSize size(parser.value(widthOption).toInt(),
               parser.value(heightOption).toInt());

    if (size.isValid())
        Options::instance.windowSize = size;

    ResultRecorder::startResults(Options::instance.id);
    ResultRecorder::recordWindowSize(Options::instance.windowSize);

    if (parser.isSet(fpsOverrideOption))
        Options::instance.fpsOverride = parser.value(fpsOverrideOption).toFloat();

    if (Options::instance.bmTemplate == QStringLiteral("sustained-fps"))
        Options::instance.bmTemplate = QStringLiteral("qrc:/Shell_SustainedFpsWithCount.qml");
    else if (Options::instance.bmTemplate == QStringLiteral("static-count"))
        Options::instance.bmTemplate = QStringLiteral("qrc:/Shell_SustainedFpsWithStaticCount.qml");
    else if (Options::instance.bmTemplate == QStringLiteral("frame-count")) {
        ResultRecorder::opsAreActuallyFrames = true;
        Options::instance.bmTemplate = QStringLiteral("qrc:/Shell_TotalFramesWithStaticCount.qml");
    }
    else
        Options::instance.bmTemplate = QStringLiteral("qrc:/Shell_SustainedFpsWithCount.qml");

    foreach (QString input, parser.positionalArguments()) {
        QFileInfo info(input);
        if (!info.exists()) {
            qWarning() << "input doesn't exist:" << input;
        } else if (info.suffix() == QStringLiteral("qml")) {
            runner.benchmarks << Benchmark(info.absoluteFilePath());
        } else if (info.isDir()) {
            QHash<QString, QString> basenameDuplicateCheck; // basename -> relative path
            QDirIterator iterator(input, QStringList() << QStringLiteral("*.qml"), QDir::NoFilter, QDirIterator::Subdirectories);
            while (iterator.hasNext()) {
                QFileInfo fi(iterator.next());

                if (basenameDuplicateCheck.contains(fi.baseName())) {
                    qWarning() << "Found basename " << fi.baseName() << " in "
                               << fi.absoluteDir().absolutePath() << "and "
                               << basenameDuplicateCheck.value(fi.baseName());
                    qWarning() << "Can't continue with a duplicate basename, as it might mess up results.";
                    exit(1);
                }

                basenameDuplicateCheck[fi.baseName()] = fi.absoluteDir().absolutePath();
                runner.benchmarks << Benchmark(fi.filePath());
            }
        }
    }

    return parser.positionalArguments();
}

void setupDefaultSurfaceFormat(int argc, char **argv)
{
    bool expectingShell = false;
    for (int i = 0; i < argc; ++i) {
        if (strcmp(argv[i], "--shell")) {
            expectingShell = true;
        } else if (expectingShell && strcmp(argv[i], "frame-count") == 0) {
            QSurfaceFormat format = QSurfaceFormat::defaultFormat();
#if QT_VERSION >= 0x050300
            format.setSwapInterval(0);
#else
            fprintf(stderr, "Cannot disable swap interval on this Qt version, frame-count shell won't work properly!\n");
            ::exit(1);
#endif
            QSurfaceFormat::setDefaultFormat(format);
        } else {
            expectingShell = false;
        }
    }
}

int main(int argc, char **argv)
{
    // We need to do this early on, so there's no interference from the shared
    // GL context.
    setupDefaultSurfaceFormat(argc, argv);

    qmlRegisterType<QQuickView>();

    QGuiApplication app(argc, argv);
    BenchmarkRunner runner;
    QStringList positionalArgs = processCommandLineArguments(app, runner);

    if (Options::instance.verbose && !Options::instance.isSubProcess) {
        std::cout << "Frame Rate .........: " << (Options::instance.fpsOverride > 0 ? Options::instance.fpsOverride : QGuiApplication::primaryScreen()->refreshRate()) << std::endl;
        std::cout << "Fullscreen .........: " << (Options::instance.fullscreen ? "yes" : "no") << std::endl;
        std::cout << "Fullscreen .........: " << (Options::instance.fullscreen ? "yes" : "no") << std::endl;
        std::cout << "Fps Interval .......: " << Options::instance.fpsInterval << std::endl;
        std::cout << "Fps Tolerance ......: " << Options::instance.fpsTolerance << std::endl;
        std::cout << "Repetitions ........: " << Options::instance.repeat;
        std::cout << std::endl;
        std::cout << "Template ...........: " << Options::instance.bmTemplate.toStdString() << std::endl;
        std::cout << "Benchmarks:" << std::endl;
        foreach (const Benchmark &b, runner.benchmarks) {
            std::cout << " - " << b.fileName.toStdString() << std::endl;
        }
    }

    int ret = 0;

    // qmlbench works as a split process mode. The parent process
    // (!isSubProcess) proxies a bunch of child processes that actually do the
    // work, and report output back to the parent. This keeps the benchmark
    // environment fairly clean.
    if (!Options::instance.isSubProcess) {
        QStringList sanitizedArgs;

        // The magic sauce
        sanitizedArgs.append("--silently-really-run-and-bypass-subprocess");

        // Add everything that was not a file/dir
        for (const QString &arg : qApp->arguments()) {
            if (!positionalArgs.contains(arg) && arg != argv[0])
                sanitizedArgs.append(arg);
        }

        ret = 0;

        for (const Benchmark &b : runner.benchmarks) {
            QStringList sanitizedArgCopy = sanitizedArgs;
            sanitizedArgCopy.append(b.fileName);

            QProcess *p = new QProcess;
            QObject::connect(p, &QProcess::readyReadStandardError, p, [&]() {
                QStringList lines = QString::fromLatin1(p->readAllStandardError()).split("\n");
                for (const QString &ln : lines) {
                    if (!ln.isEmpty())
                        std::cerr << "SUB: " << ln.toLocal8Bit().constData() << "\n";
                }
            });

            QByteArray jsonOutput;

            QObject::connect(p, &QProcess::readyReadStandardOutput, p, [&]() {
                QStringList lines = QString::fromLatin1(p->readAllStandardOutput()).split("\n");
                for (const QString &ln : lines) {
                    if (!Options::instance.onlyPrintJson) {
                        if (!ln.isEmpty())
                            std::cout << "SUB: " << ln.toLocal8Bit().constData() << "\n";
                    } else {
                        jsonOutput += ln.toUtf8();
                    }
                }
            });

            if (ret == 0) {
                p->start(argv[0], sanitizedArgCopy);
                if (!p->waitForFinished(60*10*1000)) {
                    qWarning() << "Test hung (probably indefinitely) indefinitely when run with arguments: " << sanitizedArgCopy.join(' ');
                    qWarning("Aborting test run, as this probably means benchmark setup is screwed up or the hardware needs resetting!");

                    // Don't exit straight away. Instead, record empty runs for
                    // everything else (so this is visualized as being a problem),
                    // and then exit uncleanly to allow the harness to restart the HW.
                    ret = 1;
                }

                if (p->exitStatus() != QProcess::NormalExit) {
                    qWarning() << "Test crashed when run with arguments: " << sanitizedArgCopy.join(' ');

                    // Continue the run, but note the failure.
                }
            }
            delete p;

            if (Options::instance.onlyPrintJson) {
                // Turn stdout into a JSON object and merge our results into the
                // final ones.
                QJsonParseError jerr;
                QJsonDocument d = QJsonDocument::fromJson(jsonOutput, &jerr);
                if (d.isNull()) {
                    qWarning() << "Can't parse JSON for result for " << b.fileName;
                    qWarning() << "Error: " << jerr.errorString();
                } else {
                    QJsonObject o = d.object();

                    /* skip the "wrapper" object, as mergeResults
                     * will create a new one itself.
                     */
                    o = o.begin()->toObject();
                    ResultRecorder::mergeResults(b.fileName, o);
                }
            }
        }
    } else {
        // Subprocess mode... Simple :)
        if (!runner.execute())
            return 0;

        ret = app.exec();
    }

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
    QTimer::singleShot(Options::instance.delayedStart, this, SLOT(start()));

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

    m_view->resize(Options::instance.windowSize);

    if (Options::instance.fullscreen)
        m_view->showFullScreen();
    else
        m_view->show();
    m_view->raise();

    return true;
}

void BenchmarkRunner::start()
{
    Benchmark &bm = benchmarks[m_currentBenchmark];

    if (bm.operationsPerFrame.size() == 0 && !Options::instance.onlyPrintJson)
        std::cout << "running: " << bm.fileName.toStdString() << std::endl;

    m_component = new QQmlComponent(m_view->engine(), bm.fileName);
    if (m_component->status() != QQmlComponent::Ready) {
        qWarning() << "component is not ready" << bm.fileName;
        foreach (const QQmlError &error, m_component->errors()) {
            qWarning() << "ERROR: " << error;
        }
        abort();
        return;
    }

    m_view->setSource(QUrl(Options::instance.bmTemplate));
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
        if (!Options::instance.onlyPrintJson)
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

qreal stddev(qreal avg, const QList<qreal> &list)
{
    qreal dev = 0;
    foreach (qreal v, list)
        dev += (v - avg) * (v - avg);
    return ::sqrt(dev / list.size());
}

qreal average(const QList<qreal> &list)
{
    qreal avg = 0;
    foreach (qreal r, list)
        avg += r;
    avg /= list.size();
    return avg;
}

void BenchmarkRunner::recordOperationsPerFrame(qreal ops)
{
    Benchmark &bm = benchmarks[m_currentBenchmark];
    bm.operationsPerFrame << ops;
    ResultRecorder::recordOperationsPerFrame(bm.fileName, ops);

    QList<qreal> results = bm.operationsPerFrame;
    qreal avg = average(results);

    if (results.size() >= Options::instance.repeat) {
        std::sort(results.begin(), results.end());
        qreal median = results.at(results.size() / 2);
        ResultRecorder::recordOperationsPerFrameAverage(bm.fileName, avg, bm.operationsPerFrame.size(), stddev(avg, results), median);
    }

    complete();
}

void BenchmarkRunner::complete()
{
    m_component->deleteLater();
    m_component = 0;

    bool restart = false;
    restart = benchmarks[m_currentBenchmark].operationsPerFrame.size() < Options::instance.repeat;

    if (restart)
        QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection);
    else
        QMetaObject::invokeMethod(this, "maybeStartNext", Qt::QueuedConnection);
}

#include "main.moc"
