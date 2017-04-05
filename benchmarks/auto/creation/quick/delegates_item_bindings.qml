import QtQuick 2.0
import QmlBench 1.0

// Compare with delegates_item_states
CreationBenchmark {
    id: root;
    count: 50
    staticCount: 5000
    delegate: Item {
        id: itemInstance
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
        visible: 1 == 1 ? true : false
    }
}

