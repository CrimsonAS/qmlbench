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

#include <QTimer>
#include <QGuiApplication>
#include <QQmlContext>
#include <QDebug>

#include <cmath>
#include <iostream>

#include "resultrecorder.h"
#include "benchmarkrunner.h"

BenchmarkRunner::BenchmarkRunner()
    : m_view(0)
{
}

BenchmarkRunner::~BenchmarkRunner()
{
}

void BenchmarkRunner::createView()
{
    Q_ASSERT(m_view == 0);
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
}

bool BenchmarkRunner::execute()
{
    if (Options::instance.benchmarks.size() == 0)
        return false;

    if (Options::instance.destroyViewEachRun) {
        qWarning() << "Deleting the view. Remember, you should only do this as a debugging aid!";
        delete m_view;
        m_view = 0;
    }
    createView();
    QTimer::singleShot(Options::instance.delayedStart, this, SLOT(start()));
    return true;
}

void BenchmarkRunner::start()
{
    Benchmark &bm = Options::instance.benchmarks.first();

    if (bm.operationsPerFrame.size() == 0)
        std::cerr << "running: " << bm.fileName.toStdString() << std::endl;

    m_component = new QQmlComponent(m_view->engine(), bm.fileName);
    if (m_component->status() != QQmlComponent::Ready) {
        qWarning() << "component is not ready" << bm.fileName;
        for (const QQmlError &error : m_component->errors()) {
            qWarning() << "ERROR: " << error;
        }
        abort();
        return;
    }

    m_view->setSource(QUrl(Options::instance.bmTemplate));
    if (!m_view->rootObject()) {
        qWarning() << "no root object..";
        abort();
        return;
    }
}

void BenchmarkRunner::finished()
{
    delete m_view;
    m_view = 0;
    std::cerr << "All done..." << std::endl;
    qApp->quit();
}

void BenchmarkRunner::abort()
{
    qWarning() << "Aborting...";
    exit(1);
}

// Returns "corrected sample standard deviation".
qreal stddev(qreal avg, const QList<qreal> &list)
{
    qreal dev = 0;
    for (qreal v : list)
        dev += (v - avg) * (v - avg);

    // Note: - 1 is intentional!
    return std::sqrt(dev / (list.size() - 1));
}

qreal average(const QList<qreal> &list)
{
    qreal avg = 0;
    for (qreal r : list)
        avg += r;
    avg /= list.size();
    return avg;
}

void BenchmarkRunner::recordOperationsPerFrame(qreal ops)
{
    Benchmark &bm = Options::instance.benchmarks.first();
    bm.operationsPerFrame << ops;
    ResultRecorder::recordOperationsPerFrame(ops);

    QList<qreal> results = bm.operationsPerFrame;
    qreal avg = average(results);

    if (results.size() >= Options::instance.repeat) {
        std::sort(results.begin(), results.end());
        qreal median = results.at(results.size() / 2);
        ResultRecorder::recordOperationsPerFrameAverage(avg, bm.operationsPerFrame.size(), stddev(avg, results), median);
    }

    m_component->deleteLater();
    m_component = 0;

    bool restart = false;
    restart = bm.operationsPerFrame.size() < Options::instance.repeat;

    if (restart) {
        if (Options::instance.destroyViewEachRun)
            QMetaObject::invokeMethod(this, "execute", Qt::QueuedConnection); // recreates the view too
        else
            QMetaObject::invokeMethod(this, "start", Qt::QueuedConnection); // recreates the view too
    } else {
        QMetaObject::invokeMethod(this, "finished", Qt::QueuedConnection);
    }
}


