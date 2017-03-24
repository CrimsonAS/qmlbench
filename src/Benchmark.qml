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

import QtQuick 2.0

// The base of all benchmarks. Please be careful with how much you add to this.
// It is not instantiated often (only once per "run"), but it is always present,
// so no low-duration timers etc.
Item {
    id: root

    // How many iterations to run "right now"? Note that some shells
    // (Shell_SustainedFpsWithCount.qml at least) dynamically alter this value
    // while the benchmark is running.
    property int count: 0

    // How many iterations to copy to 'count'? The 'static' shells use this
    // value.
    property int staticCount: 0

    // A boolean benchmark has only two meaningful outputs: good, or bad. An
    // contrived example of this would be a benchmark that ensured that we could
    // render more than 10 frames in 5 seconds of real-world time (in practice,
    // this isn't too useful, but there are real world cases).
    //
    // This property is simply of use to help mark these tests so they are not
    // misinterpreted.
    property bool isBooleanResult: false;

    // Used to provide a constant "tick" that benchmarks can hook into.
    property real t
    NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
}
