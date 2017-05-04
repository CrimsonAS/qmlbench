import QtQuick 2.0

// Stacks x number of opaque rectangles on top of each other.
// Will go a lot higher than blendedrect if the target hardware supprts
// [early-z](https://en.wikipedia.org/?title=Z-buffering)
Item {
    id: root;
    property int count: 16;
    property int staticCount: 0

    Repeater {
        model: root.count;
        Rectangle {
            width: root.width
            height: root.height
            color: Qt.hsla((index * .271) % 1.0, 0.5, 0.5);
        }
    }
}
