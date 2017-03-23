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

#include <QGuiApplication>
#include <QOpenGLContext>
#include <QOffscreenSurface>
#include <QOpenGLFunctions>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include <iostream>
#include <cmath>

#include "resultrecorder.h"
#include "options.h"

QVariantMap ResultRecorder::m_results;
bool ResultRecorder::opsAreActuallyFrames = false;

void ResultRecorder::startResults(const QString &id)
{
    // sub process will get all this.
    // safer that way, as then we keep OpenGL (and QGuiApplication) out of the host
    if (!Options::instance.isSubProcess)
        return;

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

void ResultRecorder::recordWindowSize(const QSize &windowSize)
{
    m_results["windowSize"] = QString::number(windowSize.width()) + "x" + QString::number(windowSize.height());
}

void ResultRecorder::recordOperationsPerFrame(const QString &benchmark, int ops)
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

void ResultRecorder::recordOperationsPerFrameAverage(const QString &benchmark, qreal ops, int samples, qreal stddev, qreal median)
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

void ResultRecorder::mergeResults(const QJsonObject &o)
{
    // Merge the keys from the subprocess in where they should be.
    for (auto it = o.constBegin(); it != o.constEnd(); ++it) {
        m_results[it.key()] = it.value().toVariant();
    }
}

void ResultRecorder::finish()
{
    if (Options::instance.onlyPrintJson) {
        QJsonDocument results = QJsonDocument::fromVariant(m_results);
        std::cout << results.toJson().constData();
    }
    m_results.clear();
}

