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

Benchmark {
    id: root;
    count: 1; // Unused
    staticCount: 1; // Unused

    width: 1280
    height: 720
    visible: true

    View3D {
        anchors.fill: parent

        environment: SceneEnvironment {
            clearColor: "#808080"
            backgroundMode: SceneEnvironment.Color
            antialiasingMode: SceneEnvironment.MSAA
            antialiasingQuality: SceneEnvironment.High
        }

        PerspectiveCamera {
            position: Qt.vector3d(0, 400, 600)
            eulerRotation.x: -30
            clipFar: 2000
        }

        DirectionalLight {
            id: light1
            color: Qt.rgba(1.0, 0.1, 0.1, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
            position: Qt.vector3d(0, 200, 0)
            rotation: Quaternion.fromEulerAngles(-135, -90, 0)
            shadowMapQuality: Light.ShadowMapQualityHigh
            visible: true
            castsShadow: true
            brightness: 0.5
            SequentialAnimation on rotation {
                loops: Animation.Infinite
                QuaternionAnimation {
                    to: Quaternion.fromEulerAngles(-45, -90, 0)
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                QuaternionAnimation {
                    to: Quaternion.fromEulerAngles(-135, -90, 0)
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
        }

        PointLight {
            id: light2
            color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
            position: Qt.vector3d(0, 300, 0)
            shadowMapFar: 2000
            shadowMapQuality: Light.ShadowMapQualityHigh
            visible: true
            castsShadow: true
            brightness: 0.5
            SequentialAnimation on x {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 400
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 0
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
        }

        PointLight {
            id: light3
            color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
            position: Qt.vector3d(0, 300, 0)
            shadowMapFar: 2000
            shadowMapQuality: Light.ShadowMapQualityHigh
            visible: true
            castsShadow: true
            brightness: 0.5
            SequentialAnimation on x {
                loops: Animation.Infinite
                NumberAnimation {
                    to: -400
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 0
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
        }

        PointLight {
            id: light2b
            color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
            position: Qt.vector3d(0, 300, 0)
            shadowMapFar: 2000
            shadowMapQuality: Light.ShadowMapQualityHigh
            visible: true
            castsShadow: true
            brightness: 0.5
            SequentialAnimation on z {
                loops: Animation.Infinite
                NumberAnimation {
                    to: 400
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 0
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
        }

        PointLight {
            id: light3b
            color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
            position: Qt.vector3d(0, 300, 0)
            shadowMapFar: 2000
            shadowMapQuality: Light.ShadowMapQualityHigh
            visible: true
            castsShadow: true
            brightness: 0.5
            SequentialAnimation on z {
                loops: Animation.Infinite
                NumberAnimation {
                    to: -400
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
                NumberAnimation {
                    to: 0
                    duration: 2000
                    easing.type: Easing.InOutQuad
                }
            }
        }
        SpotLight {
            id: light4
            color: Qt.rgba(1.0, 0.9, 0.7, 1.0)
            ambientColor: Qt.rgba(0.0, 0.0, 0.0, 0.0)
            position: Qt.vector3d(0, 250, 0)
            eulerRotation.x: -45
            shadowMapFar: 2000
            shadowMapQuality: Light.ShadowMapQualityHigh
            visible: true
            castsShadow: true
            brightness: 0.5
            coneAngle: 50
            innerConeAngle: 30
            PropertyAnimation on eulerRotation.y {
                loops: Animation.Infinite
                from: 0
                to: -360
                duration: 10000
            }
        }

        Model {
            source: "#Rectangle"
            y: -200
            scale: Qt.vector3d(15, 15, 15)
            eulerRotation.x: -90
            materials: [
                DefaultMaterial {
                    diffuseColor: Qt.rgba(0.8, 0.6, 0.4, 1.0)
                }
            ]
        }
        Model {
            source: "#Rectangle"
            z: -400
            scale: Qt.vector3d(15, 15, 15)
            materials: [
                DefaultMaterial {
                    diffuseColor: Qt.rgba(0.8, 0.8, 0.9, 1.0)
                }
            ]
        }

        Model {
            source: "#Sphere"
            scale: Qt.vector3d(3, 3, 3)
            materials: [
                DefaultMaterial {
                    diffuseColor: Qt.rgba(0.9, 0.9, 0.9, 1.0)
                }
            ]
        }

        Model {
            source: "#Cube"
            position: light1.position
            rotation: light1.rotation
            property real size: 0.2
            scale: Qt.vector3d(size, size, size)
            materials: [
                DefaultMaterial {
                    diffuseColor: light1.color
                    opacity: 0.4
                }
            ]
        }
        Model {
            source: "#Cube"
            position: light2.position
            rotation: light2.rotation
            property real size: 0.2
            scale: Qt.vector3d(size, size, size)
            materials: [
                DefaultMaterial {
                    diffuseColor: light2.color
                    opacity: 0.4
                }
            ]
        }
        Model {
            source: "#Cube"
            position: light3.position
            rotation: light3.rotation
            property real size: 0.2
            scale: Qt.vector3d(size, size, size)
            materials: [
                DefaultMaterial {
                    diffuseColor: light3.color
                    opacity: 0.4
                }
            ]
        }
        Model {
            source: "#Cube"
            position: light2b.position
            rotation: light2b.rotation
            property real size: 0.2
            scale: Qt.vector3d(size, size, size)
            materials: [
                DefaultMaterial {
                    diffuseColor: light2b.color
                    opacity: 0.4
                }
            ]
        }
        Model {
            source: "#Cube"
            position: light3b.position
            rotation: light3b.rotation
            property real size: 0.2
            scale: Qt.vector3d(size, size, size)
            materials: [
                DefaultMaterial {
                    diffuseColor: light3b.color
                    opacity: 0.4
                }
            ]
        }
        Model {
            source: "#Cube"
            position: light4.position
            rotation: light4.rotation
            property real size: 0.2
            scale: Qt.vector3d(size, size, size)
            materials: [
                DefaultMaterial {
                    diffuseColor: light4.color
                    opacity: 0.4
                }
            ]
        }
    }
}
