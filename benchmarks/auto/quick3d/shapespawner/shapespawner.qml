/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the test suite of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick
import QtQuick3D
import QmlBench

CreationBenchmark {
    id: root;
    count: 4000;
    staticCount: 4000;

    View3D {
        id: view
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "skyblue"
            backgroundMode: SceneEnvironment.Color
        }

        PerspectiveCamera {
            position: Qt.vector3d(0, 200, 300)
            eulerRotation.x: -30
        }

        DirectionalLight {
            eulerRotation.x: -30
            eulerRotation.y: -70
        }

        Node {
            id: shapeSpawner
            property real range: 300
            property var instances: []
            property int count

            function addShapes()
            {
                for (var i = 0; i < count; i++) {
                    var xPos = (2 * Math.random() * range) - range;
                    var yPos = (2 * Math.random() * range) - range;
                    var zPos = (2 * Math.random() * range) - range;
                    var shapeComponent = Qt.createComponent("WeirdShape.qml");
                    let instance = shapeComponent.createObject(shapeSpawner,
                        { "x": xPos, "y": yPos, "z": zPos, "scale": Qt.vector3d(0.05, 0.05, 0.05)});
                    instances.push(instance);
                }
            }

            function removeShapes()
            {
                for (var i = 0; i < instances.length; i++) {
                    instances[i].destroy();
                }
                instances = [];
            }

            Connections {
                target: root
                function onCountChanged() {
                    shapeSpawner.count = root.count;
                    shapeSpawner.removeShapes();
                    shapeSpawner.addShapes();
                }
            }
        }

        Component.onCompleted: {
            shapeSpawner.count = count;
            shapeSpawner.addShapes();
        }
    }
}
