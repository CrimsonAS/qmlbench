import QtQuick 2.0

Item {
    id: root;
    property int count: 50;

    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
    onTChanged: {
        allocate();
    }

    Component {
        id: component;
        Item {
        }
    }

    function allocate() {
        for (var i=0; i<root.count; ++i) {
            var object = component.createObject();
            object.destroy();
        }
    }
}
