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

    property var names: [
        "butterfly-wide.png",
        "butterfly-half.png",
        "butterfly-collapsed.png"
    ];

    Repeater {
        id: repeater
        Image {
            x: Math.random() * (root.width - width)
            y: Math.random() * (root.height - height)
            source: "../" + root.names[index % 3];
            width: 20
            height: 20

        }
    }
}
