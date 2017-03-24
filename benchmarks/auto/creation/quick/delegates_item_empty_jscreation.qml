import QtQuick 2.0
import QmlBench 1.0

Benchmark {
    id: root;
    count: 50;
    staticCount: 5000;

    property var items;
    onTChanged: {
        allocate();
    }

    Component {
        id: component;
        Item {
        }
    }

    function allocate() {
        if (items && items.length) {
            for (var i=0; i<items.length; ++i)
                items[i].destroy();
        }
        items = [];

        for (var i=0; i<root.count; ++i) {
            var object = component.createObject(root);
            items.push(object);
        }
    }
}
