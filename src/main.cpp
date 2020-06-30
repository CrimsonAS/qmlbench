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

#include "benchmark.h"
#include "benchmarkrunner.h"
#include "options.h"
#include "resultrecorder.h"
#include "qcommandlineparser.h"
#include "testmodel.h"
#include "qmlbench.h"

Options Options::instance;

static const char *subprocessOptionString = "subprocess-mode";

QStringList processCommandLineArguments(const QCoreApplication &app)
{
    QCommandLineParser parser;

    QCommandLineOption subprocessOption(subprocessOptionString);

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

    QCommandLineOption timeoutOption(QStringLiteral("timeout"),
                                     QStringLiteral("Sets the maximum time in milliseconds that a test is allowed to run before it is aborted. The default is 10 minutes. Set this to -1 to disable the timeout."),
                                     QStringLiteral("timeout"),
                                     QStringLiteral("600000"));
    parser.addOption(timeoutOption);

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

    QCommandLineOption destroyViewOption(QStringLiteral("destroy-view"),
                                         QStringLiteral("Destroys the QQuickView between each test run. Use it as a debug aid, do not benchmark with this!"));
    parser.addOption(destroyViewOption);

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
        Options::instance.printJsonToStdout = true;
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
    Options::instance.destroyViewEachRun = parser.isSet(destroyViewOption);
    Options::instance.timeout = parser.value(timeoutOption).toInt();

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
    else { // "frame-count" and also the default option now
        ResultRecorder::opsAreActuallyFrames = true;
        Options::instance.bmTemplate = QStringLiteral("qrc:/Shell_TotalFramesWithStaticCount.qml");
    }

    foreach (QString input, parser.positionalArguments()) {
        QFileInfo info(input);
        if (!info.exists()) {
            qWarning() << "input doesn't exist:" << input;
        } else if (info.suffix() == QStringLiteral("qml")) {
            Options::instance.benchmarks << Benchmark(info.absoluteFilePath());
        } else if (info.isDir()) {
            QHash<QString, QString> basenameDuplicateCheck; // basename -> relative path
            QDirIterator iterator(input, QStringList() << QStringLiteral("*.qml"), QDir::NoFilter, QDirIterator::Subdirectories);
            while (iterator.hasNext()) {
                QFileInfo fi(iterator.next());

                if (fi.path().contains("/manual/") && !info.filePath().contains("/manual")) {
                    qDebug() << "Skipping unrequested manual test " << fi.absoluteFilePath();
                    continue;
                }

                if (basenameDuplicateCheck.contains(fi.baseName())) {
                    qWarning() << "Found basename " << fi.baseName() << " in "
                               << fi.absoluteDir().absolutePath() << "and "
                               << basenameDuplicateCheck.value(fi.baseName());
                    qWarning() << "Can't continue with a duplicate basename, as it might mess up results.";
                    exit(1);
                }

                basenameDuplicateCheck[fi.baseName()] = fi.absoluteDir().absolutePath();
                Options::instance.benchmarks << Benchmark(fi.filePath());
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

int runHostProcess(const QCoreApplication &app, const QStringList &positionalArgs)
{
    int ret = 0;

    if (Options::instance.verbose) {
        if (Options::instance.fpsOverride > 0)
            std::cout << "Frame Rate .........: " << Options::instance.fpsOverride << std::endl;
        else
            std::cout << "Frame Rate .........: default" << std::endl;
        std::cout << "Fullscreen .........: " << (Options::instance.fullscreen ? "yes" : "no") << std::endl;
        std::cout << "Fullscreen .........: " << (Options::instance.fullscreen ? "yes" : "no") << std::endl;
        std::cout << "Fps Interval .......: " << Options::instance.fpsInterval << std::endl;
        std::cout << "Fps Tolerance ......: " << Options::instance.fpsTolerance << std::endl;
        std::cout << "Repetitions ........: " << Options::instance.repeat;
        std::cout << std::endl;
        std::cout << "Template ...........: " << Options::instance.bmTemplate.toStdString() << std::endl;
        std::cout << "Benchmarks:" << std::endl;
        foreach (const Benchmark &b, Options::instance.benchmarks) {
            std::cout << " - " << b.fileName.toStdString() << std::endl;
        }
    }

    QStringList sanitizedArgs;

    // The magic sauce
    sanitizedArgs.append(QLatin1String("--") + subprocessOptionString);

    // Add everything that was not a file/dir
    for (const QString &arg : qApp->arguments()) {
        if (!positionalArgs.contains(arg) && arg != app.arguments().first())
            sanitizedArgs.append(arg);
    }

    ret = 0;

    for (const Benchmark &b : Options::instance.benchmarks) {
        QStringList sanitizedArgCopy = sanitizedArgs;
        sanitizedArgCopy.append(b.fileName);

        QProcess *p = new QProcess;
        QByteArray stdErrBuf;
        QObject::connect(p, &QProcess::readyReadStandardError, p, [&]() {
            stdErrBuf += p->readAllStandardError();
            int nlIdx = 0;
            forever {
                nlIdx = stdErrBuf.indexOf('\n');
                if (nlIdx == -1)
                    break;
                QByteArray ln = stdErrBuf.left(nlIdx);
                stdErrBuf = stdErrBuf.right(stdErrBuf.size() - nlIdx - 1);
                std::cerr << ln.constData() << "\n";
            }
        });

        QByteArray stdOutBuf;

        QObject::connect(p, &QProcess::readyReadStandardOutput, p, [&]() {
            stdOutBuf += p->readAllStandardOutput();
            if (Options::instance.printJsonToStdout)
                return;
            int nlIdx = 0;
            forever {
                nlIdx = stdOutBuf.indexOf('\n');
                if (nlIdx == -1)
                    break;
                QByteArray ln = stdOutBuf.left(nlIdx);
                stdOutBuf = stdOutBuf.right(stdOutBuf.size() - nlIdx - 1);
                std::cout << ln.constData() << "\n";
            }
        });

        QObject::connect(p,
            static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
            [=](int /*exitCode*/, QProcess::ExitStatus /*exitStatus*/) {
            // Flush the leftovers (if any)
            std::cerr << stdErrBuf.constData() << "\n";
            if (!Options::instance.printJsonToStdout)
                std::cout << stdOutBuf.constData() << "\n";
        });

        if (ret == 0) {
            p->start(app.arguments().first(), sanitizedArgCopy);
            if (!p->waitForFinished(Options::instance.timeout)) {
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

        if (Options::instance.printJsonToStdout) {
            // Turn stdout into a JSON object and merge our results into the
            // final ones.
            QJsonParseError jerr;
            QJsonDocument d = QJsonDocument::fromJson(stdOutBuf, &jerr);
            if (d.isNull()) {
                qWarning() << "Can't parse JSON for result for " << b.fileName;
                qWarning() << "Error: " << jerr.errorString();
            } else {
                ResultRecorder::mergeResults(d.object());
            }
        }
    }

    return ret;
}

int runSubProcess(QGuiApplication &app)
{
    BenchmarkRunner runner;
    if (!runner.execute())
        return 0;

    return app.exec();
}

static QObject *qmlbench_singleton_provider(QQmlEngine *, QJSEngine *)
{
    return new QmlBench();
}

int main(int argc, char **argv)
{
    // We need to do this early on, so there's no interference from the shared
    // GL context.
    setupDefaultSurfaceFormat(argc, argv);

    qmlRegisterType<QQuickView>();
    qmlRegisterType(QUrl("qrc:/Benchmark.qml"), "QmlBench", 1, 0, "Benchmark");
    qmlRegisterType(QUrl("qrc:/CreationBenchmark.qml"), "QmlBench", 1, 0, "CreationBenchmark");
    qmlRegisterType(QUrl("qrc:/V8Benchmark.qml"), "QmlBench", 1, 0, "V8Benchmark");
    qmlRegisterType<TestModel>("QmlBench", 1, 0, "TestModel");
    qmlRegisterSingletonType<QmlBench>("QmlBench", 1, 0, "QmlBench", qmlbench_singleton_provider);

    QScopedPointer<QCoreApplication> app;

    bool needsGui = false;
    for (int i = 0; i < argc; ++i) {
        // strstr because the actual option looks like "--foo" not "foo".
        // not perfect, but good enough.
        if (strstr(argv[i], subprocessOptionString) != NULL) {
            needsGui = true;
            break;
        }
    }

    if (needsGui) {
        app.reset(new QGuiApplication(argc, argv));
    } else {
        app.reset(new QCoreApplication(argc, argv));
    }

    QStringList positionalArgs = processCommandLineArguments(*app);

    int ret = 0;

    // qmlbench works as a split process mode. The parent process
    // (!isSubProcess) proxies a bunch of child processes that actually do the
    // work, and report output back to the parent. This keeps the benchmark
    // environment fairly clean.
    if (!Options::instance.isSubProcess) {
        ret = runHostProcess(*app, positionalArgs);
    } else {
        ret = runSubProcess(static_cast<QGuiApplication&>(*app));
    }

    ResultRecorder::finish();
    return ret;
}

