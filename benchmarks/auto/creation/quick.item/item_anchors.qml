import QtQuick 2.0
import QmlBench 1.0

// Testing the performance of anchors against w/h bindings (item_size).
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
            anchors.fill: parent
        }
    }
}
