import QtQuick 2.0

Item {
    id: root;
    property int count: 50;

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
            x: Math.random() * (root.width - width)
            y: Math.random() * (root.height - height)
            width: 30
            height: 15
            color: "steelblue"
            Component.onCompleted: {
                var t = Math.random();
                color = Qt.hsla(t, 1, 0.5);
            }
        }
    }
}
