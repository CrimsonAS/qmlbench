import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of Item with a non-zero z.
// This is an unusually expensive property.
CreationBenchmark {
    id: root;
    count: 50
    staticCount: 5000
    delegate: Item {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        z: index
        width: 30
        height: 15
    }
}

