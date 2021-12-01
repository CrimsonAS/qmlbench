import QtQuick 2.0

// Take this one with a grain of salt. Graphics drivers have a
// lot of overhead in how drawing is set up, and seeing an individual GL call take
// up to a millisecond (yes, a millisecond) is not uncommon. This test is a highly
// constructed case to try to get a rough ballpart of how many discrete draw calls
// the GL stack is capable of.
//
// This is mostly important if you end up with an application that fails to do
// batching in the scene graph renderer, but as this situation will typically
// have many other performance problems, this may not be a useful benchmark for
// the most part.
Item {
    id: root;

    property real cellSize: Math.floor(Math.sqrt(width * height / (count / 2)))
    property int count: 250
    property int staticCount: 0

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
                    source: "qrc:///shared/butterfly-wide.png"
                }
            }
        }
    }
}
