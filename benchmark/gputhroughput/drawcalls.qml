import QtQuick 2.0

Item {
    id: root;

    property real cellSize: Math.floor(Math.sqrt(width * height / (count / 2)))
    property int count: 250

    Grid {
        width: root.width
        height: root.height
        columns: Math.ceil(root.width / root.cellSize);
        rows: Math.ceil(root.height / root.cellSize);
        Repeater {
            model: root.count / 2
            Rectangle {
                opacity: 0.9
                width: root.cellSize
                height: root.cellSize
                color: Qt.hsla(index / count, 0.5, 0.5);
                Image {
                    // partially overlap the next cells to provoke separate draw calls.
                    x: parent.width / 2
                    y: parent.height / 2
                    width: x + 2
                    height: y + 2
                    sourceSize: Qt.size(width, height);
                    source: "../butterfly-wide.png"
                }
            }
        }
    }
}
