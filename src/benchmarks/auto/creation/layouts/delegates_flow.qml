import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of a Flow layout
CreationBenchmark {
    id: root;
    count: 20;
    staticCount: 1000;
    delegate: Flow {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        Rectangle {
            width: 50
            height: 10
            color: "red"
        }
        Rectangle {
            width: 50
            height: 10
            color: "red"
        }
        Rectangle {
            width: 50
            height: 10
            color: "red"
        }
        Rectangle {
            width: 50
            height: 10
            color: "red"
        }
        Rectangle {
            width: 50
            height: 10
            color: "red"
        }
    }
}
