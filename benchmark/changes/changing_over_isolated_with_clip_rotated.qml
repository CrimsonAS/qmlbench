import QtQuick 2.0

Item {
    id: root;
    property int count: 1000;
    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 2347; loops: Animation.Infinite }

    Item {
        anchors.fill: parent
        clip: true
        Repeater {
            id: repeater
            model: root.count

            Rectangle {
                color: Qt.hsla(Math.random(), 0.9, 0.4)
                width: 20
                height: 20
                clip: true
                rotation: 10
                x: Math.random() * root.width
                y: Math.random() * root.height

                Text {
                    color: "white"
                    text: "X"
                    anchors.centerIn: parent
                    font.pixelSize: 10
                }
            }
        }
    }

    Item {
        anchors.fill: parent
        clip: true
        Text {
            anchors.centerIn: parent
            text: "test: " + root.t;
        }
    }

}
