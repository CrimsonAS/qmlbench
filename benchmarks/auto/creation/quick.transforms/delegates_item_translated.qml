import QtQuick 2.0
import QmlBench 1.0

// Translate an item.
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Item {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
        transform: Translate {
            x: 10
            y: 10
        }
    }
}




