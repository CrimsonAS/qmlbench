import QtQuick 2.2

Item {
    id: root;

    property int size: Math.sqrt(width * height / count);
    property string description: count + " " + size + "x" + size + " Image instances\nAnimation with JavaScript";

    property int count: 500

    Grid {
        width: root.width
        height: root.height
        columns: Math.ceil(root.width / root.size);
        rows: Math.ceil(root.height / root.size);
        Repeater {
            model: root.count
            Image {
                source: "../butterfly-wide.png"
                sourceSize: Qt.size(root.size, root.size);

                property real t;
                rotation: 10 * Math.sin(t * Math.PI * 2 + Math.PI);

                SequentialAnimation on t {
                    PauseAnimation { duration: 200 + Math.random() * 200 }
                    NumberAnimation { from: 0; to: 1; duration: 1000; }
                    loops: Animation.Infinite
                }
            }
        }
    }
}
