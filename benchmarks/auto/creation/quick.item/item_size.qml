import QtQuick 2.0
import QmlBench 1.0

// Testing the performance of w/h bindings against anchors (item_anchors).
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Item {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15

        Item {
            width: parent.width
            height: parent.height
        }
    }
}

