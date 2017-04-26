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

#ifndef BENCHMARKRUNNER_H
#define BENCHMARKRUNNER_H

#include <QObject>
#include <QQuickView>
#include <QQmlComponent>
#include <QScreen>

#include "options.h"

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

    void createView();

    QQuickView *view() const { return m_view; }
    QQmlComponent *component() const { return m_component; }
    qreal screenRefreshRate() const { return Options::instance.fpsOverride > 0 ? Options::instance.fpsOverride : m_view->screen()->refreshRate(); }
    QString input() const { return Options::instance.benchmarks.first().fileName; }

    qreal fpsTolerance() const { return Options::instance.fpsTolerance / 100.0; }
    qreal fpsInterval() const { return Options::instance.fpsInterval; }

    int frameCountInterval() const { return Options::instance.frameCountInterval; }

    bool verbose() const { return Options::instance.verbose; }

    int count() const { return Options::instance.count; }

    double hardwareMultiplier() const { return Options::instance.hardwareMultiplier; }

public slots:
    bool execute();
    void recordOperationsPerFrame(qreal count);
    void abort();

private slots:
    void start();
    void finished();

private:
    QQuickView *m_view;
    QQmlComponent *m_component;
};

#endif // BENCHMARKRUNNER_H

