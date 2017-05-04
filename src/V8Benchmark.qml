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
import QmlBench 1.0
import "qrc:/3rdparty/v8-bench.js" as V8Bench

// This is a harness suited for running a V8Bench suite and reporting a number.
// We replace most of V8Bench's BenchmarkSuite (specifically, the step, run, etc
// behavior) and do our own setup, running and teardown.
//
// This is a little unfortunate as it means that we can't report results that
// directly compare with v8-bench.js, but hopefully our results will be more
// stable (as we have less magic in how many iterations we run), and we shall
// still provide useful information if a test regresses.
Benchmark {
    id: root

    // The desired suite name (e.g. "Richards"). This is then found, and put
    // into suite.
    property var suiteName

    // The found suite object
    property var suite

    Component.onCompleted: {
        var suites = V8Bench.BenchmarkSuite.suites
        // Find suite
        for (var i = 0; i < suites.length; ++i) {
            var potentialSuite = suites[i]
            if (potentialSuite.name == root.suiteName) {
                root.suite = potentialSuite
                break;
            }
        }

        // Set it up
        for (var i = 0; i < suite.benchmarks.length; ++i) {
            var benchmark = suite.benchmarks[i]
            benchmark.Setup();

            // And do a warmup iteration
            benchmark.run();
        }
    }
    Component.onDestruction: {
        // Tear it down
        for (var i = 0; i < suite.benchmarks.length; ++i) {
            var benchmark = suite.benchmarks[i]
            benchmark.TearDown();
        }
    }

    // Run a tick of the test
    onTChanged: {
        for (var i = 0; i < suite.benchmarks.length; ++i) {
            var benchmark = suite.benchmarks[i]

            // Do a few iterations, so we report a meaningful number.
            for (var j = 0; j < count; ++j) {
                benchmark.run();
            }
        }
    }
}
