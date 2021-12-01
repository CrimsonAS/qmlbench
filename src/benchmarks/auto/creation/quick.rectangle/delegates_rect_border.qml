import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of rectangles with a border
// Compare with delegates_rect
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Rectangle {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        width: 30
        height: 15
        color: "steelblue"
        border.width: 5
        border.color: "#000000"
    }
}

