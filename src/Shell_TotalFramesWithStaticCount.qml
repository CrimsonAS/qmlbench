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

Item {
    id: root
    width: 320
    height: 480

    Component.onCompleted: {
        var object
        if (benchmark.count != -1) {
            object = benchmark.component.createObject(benchmarkRoot, { count: benchmark.count });
        } else {
            object = benchmark.component.createObject(benchmarkRoot);
            if (!object.hasOwnProperty("count")) {
                print(" - error: property 'count' is lacking from: " + benchmark.input);
                benchmark.abort();
            }

            if (object.hasOwnProperty("staticCount")) {
                object.count = object.staticCount * benchmark.hardwareMultiplier
            }
        }
        object.anchors.fill = benchmarkRoot;
        root.item = object;
        root.count = 0;
    }

    property Item item;
    property int count: 0

    Item {
        id: benchmarkRoot
        clip: true
        anchors.fill: parent
    }

    Item {
        id: meter

        clip: true
        width: root.width
        height: 14
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Rectangle {
            id: swapTest
            anchors.right: parent.right
            width: parent.height
            height: parent.height
            property real t;
            NumberAnimation on t {
                id: swapAnimation
                from: 0;
                to: 1;
                duration: 1000;
                loops: Animation.Infinite
            }
            property bool inv;
            property int countDown: 5;
            property var startedCounting
            onTChanged: {
                // Run a small countdown for the first few frames so that
                // benchmarks that test  animation (without creation) has a
                // chance to stabilize before we start measuring
                if (countDown > 0) {
                    --countDown;
                    return;
                } else if (countDown == 0) {
                    --countDown;
                    startedCounting = Date.now();
                }

                if (Date.now() - startedCounting >= benchmark.frameCountInterval) {
                    // Stop the 't' ticker. Otherwise we may accidentally begin one
                    // more interval, since recordOperationsPerFrame will quit us on
                    // a queued signal.
                    swapAnimation.running = false
                    benchmark.recordOperationsPerFrame(root.count);
                } else {
                    ++root.count;
                    inv = !inv;
                }
            }
            color: inv ? "red" : "blue"
        }
    }

}
