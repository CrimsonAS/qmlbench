import QtQuick 2.0

// Stacks x number of alphablended rectangles on top of each other.
// Rough test of fillrate.
Item {
    id: root;
    property int count: 10
    property int staticCount: 0

    Repeater {
        model: root.count;
        Rectangle {
            width: root.width
            height: root.height
            color: Qt.hsla((index * .271) % 1.0, 0.5, 0.5, 0.5);
        }
    }
}
