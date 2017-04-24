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

#ifndef OPTIONS_H
#define OPTIONS_H

#include <QString>
#include <QSize>

#include "benchmark.h"

struct Options
{
    Options()
        : fullscreen(false)
        , verbose(false)
        , printJsonToStdout(false)
        , isSubProcess(false)
        , repeat(1)
        , delayedStart(0)
        , count(-1)
        , frameCountInterval(20000)
        , fpsTolerance(0.05)
        , fpsInterval(1000)
        , fpsOverride(0)
        , windowSize(800, 600)
        , hardwareMultiplier(1.0)
    {
    }

    QString bmTemplate;
    QString id;
    bool fullscreen;
    bool verbose;
    bool printJsonToStdout;
    bool isSubProcess;
    int repeat;
    int delayedStart;
    int count;
    int frameCountInterval;
    qreal fpsTolerance;
    qreal fpsInterval;
    qreal fpsOverride;
    qreal targetFps;
    QSize windowSize;
    double hardwareMultiplier;
    QList<Benchmark> benchmarks;

    static Options instance;
};

#endif // OPTIONS_H
