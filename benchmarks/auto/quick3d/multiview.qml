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
    id: root
    count: 25*25
    staticCount: 25*25

    width: 800
    height: 600
    visible: true

    // The root scene
    Node {
        id: standAloneScene
        DirectionalLight {
            ambientColor: Qt.rgba(0.5, 0.5, 0.5, 1.0)
            brightness: 1.0
            eulerRotation.x: -25
        }

        Model {
            source: "#Cube"
            y: -104
            scale: Qt.vector3d(3, 3, 0.1)
            eulerRotation.x: -90
            materials: [
                DefaultMaterial {
                    diffuseColor: Qt.rgba(0.8, 0.8, 0.8, 1.0)
                }
            ]
        }

        Model {
            source: "#Sphere"
            scale: Qt.vector3d(1, 1, 1)
            materials: [
                PrincipledMaterial {
                    baseColor: "#41cd52"
                    metalness: 0.0
                    roughness: 0.1
                    opacity: 1.0
                }
            ]
            PropertyAnimation on eulerRotation.y {
                loops: Animation.Infinite
                duration: 5000
                to: 0
                from: -360
            }
        }

        // The predefined cameras. They have to be part of the scene, i.e. inside the root node.
        // Animated perspective camera
        Node {
            PerspectiveCamera {
                id: cam1
                z: 600
            }
            PropertyAnimation on eulerRotation.x {
                loops: Animation.Infinite
                duration: 5000
                to: -360
                from: 0
            }
        }

        // Stationary perspective camera
        PerspectiveCamera {
            id: cam2
            z: 600
        }

        // Second animated perspective camera
        Node {
            PerspectiveCamera {
                id: cam3
                x: 500
                eulerRotation.y: 90
            }
            PropertyAnimation on eulerRotation.y {
                loops: Animation.Infinite
                duration: 5000
                to: 0
                from: -360
            }
        }

        PerspectiveCamera {
            id: cam4
            y: 600
            eulerRotation.x: -90
        }

        PerspectiveCamera {
            id: cam5
            x: -600
            eulerRotation.y: -90
        }

        Node {
            id: shapeSpawner
            property var instances: []
            property int count

            function addShapes()
            {
                let rows = Math.ceil(Math.sqrt(count));
                let strideX = root.width/rows;
                let strideY = root.height/rows;
                let cameraNames = ["cam1", "cam2", "cam3", "cam4", "cam5"]
                var numAdded = 0;

                for (var i = 0; i < rows; i++) {
                    for (var j = 0; j < rows; j++) {
                        if (numAdded == count)
                            break;

                        let xPos = i * strideX;
                        let yPos = j * strideY;

                        let idx = numAdded % 5;
                        let cameraName = cameraNames[idx];

                        var qmlStr =
                        [" import QtQuick ",
                        " import QtQuick3D ",
                        " ",
                        " Rectangle { ",
                        "     width: " + strideX,
                        "     height: " + strideY,
                        "     x: " + xPos,
                        "     y: " + yPos,
                        "     color: \"#848895\" ",
                        "     View3D { ",
                        "         anchors.fill: parent ",
                        "         importScene: standAloneScene ",
                        "         camera: " + cameraName,
                        "     } ",
                        " } "].join('\n');

                        var instance = Qt.createQmlObject(qmlStr, root, "View3D_" + idx);

                        instances.push(instance);
                        numAdded += 1;
                    }
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
