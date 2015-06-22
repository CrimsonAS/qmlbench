import QtQuick 2.2

Item {
    id: root;

    property real cellSize: Math.floor(Math.sqrt(width * height / count))
    property size srcSize: Qt.size(cellSize, cellSize);

    property int count: 200

    Grid {
        width: root.width
        height: root.height
        columns: Math.ceil(root.width / root.cellSize);
        rows: Math.ceil(root.height / root.cellSize);
        Repeater {
            model: root.count

            Item {
                id: sprite
                width: root.cellSize
                height: root.cellSize

                Image { id: imgWide; anchors.fill: parent; visible: false; source: "../butterfly-wide.png"; sourceSize: root.srcSize }
                Image { id: imgHalf; anchors.fill: parent; visible: false; source: "../butterfly-half.png"; sourceSize: root.srcSize }
                Image { id: imgSmall; anchors.fill: parent; visible: false; source: "../butterfly-collapsed.png"; sourceSize: root.srcSize }

                SequentialAnimation {
                    running: true
                    PropertyAction { target: imgWide; property: "visible"; value: true; }
                    PauseAnimation { duration: Math.random() * 500; }
                    PauseAnimation { duration: 300 }
                    PropertyAction { target: imgWide; property: "visible"; value: false; }
                    PropertyAction { target: imgHalf; property: "visible"; value: true; }
                    PauseAnimation { duration: 300 }
                    PropertyAction { target: imgHalf; property: "visible"; value: false; }
                    PropertyAction { target: imgSmall; property: "visible"; value: true; }
                    PauseAnimation { duration: 300 }
                    PropertyAction { target: imgSmall; property: "visible"; value: false; }

                    loops: Animation.Infinite
                }
            }
        }
    }
}
