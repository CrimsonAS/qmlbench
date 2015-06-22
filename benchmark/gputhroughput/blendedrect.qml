import QtQuick 2.0

Item {
    id: root;
    property int count: 10
    Repeater {
        model: root.count;
        Rectangle {
            width: root.width
            height: root.height
            color: Qt.hsla((index * .271) % 1.0, 0.5, 0.5, 0.5);
        }
    }
}
