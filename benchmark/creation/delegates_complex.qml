import QtQuick 2.0

Item {
    id: root;
    property int count: 20;

    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
    onTChanged: {
        repeater.model = 0;
        repeater.model = root.count
    }

    Component.onCompleted: repeater.model = root.count

    Repeater {
        id: repeater
        Rectangle {
            x: Math.random() * root.width
            y: Math.random() * root.height
            width: 100
            height: 50
            gradient: Gradient {
                GradientStop { position: 0; color: "steelblue" }
                GradientStop { position: 1; color: "black" }
            }
            Repeater {
                model: 10
                Text {
                    id: label
                    x: (index % 2) * 50
                    y: Math.floor(index / 2) * 10;
                    width: 50
                    height: 10
                    horizontalAlignment: Text.AlignHCenter
                    text: "Item #" + index
                    color: "white"
                    font.pixelSize: 10
                }
            }
        }
    }
}
