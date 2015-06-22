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
        Item {
        }
    }
}
