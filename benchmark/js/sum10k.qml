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

    function sum(n) {
        var x = 0;
        for (var i=0; i<n; ++i)
            x = x + x;
        return x;
    }

    Repeater {
        id: repeater
        Item {
            x: sum(10000);
        }
    }
}
