import QtQuick 2.0

Item {
    id: root;
    property int count: 100;
    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 2347; loops: Animation.Infinite }

    Repeater {
        id: repeater
        model: root.count
        Text {
            x: Math.random() * root.width
            y: Math.random() * root.height
            text: Math.floor( root.t * 1000 ) / 1000;
        }
    }
}
