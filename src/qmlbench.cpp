/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
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

#include "qmlbench.h"

#define PREGENERATED_COUNT 10001

QmlBench::QmlBench(QObject *parent)
    : QObject(parent)
    , m_currentRandomNumberIndex(0)
{
    m_pregeneratedRandomNumbers = new qreal[PREGENERATED_COUNT];
    for (int i = 0; i < PREGENERATED_COUNT; ++i)
        m_pregeneratedRandomNumbers[i] = qrand() / qreal(RAND_MAX);
}

QmlBench::~QmlBench()
{
    delete[] m_pregeneratedRandomNumbers;
}

qreal QmlBench::getRandom()
{
    return m_pregeneratedRandomNumbers[(m_currentRandomNumberIndex++) % PREGENERATED_COUNT];
}
