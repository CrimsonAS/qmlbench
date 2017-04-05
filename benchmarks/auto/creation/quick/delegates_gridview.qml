import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of GridView with a few simple props.
CreationBenchmark {
    id: root;
    count: 50
    staticCount: 2500
    delegate: GridView {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
    }
}

