import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of a Grid, to be compared with GridLayout
CreationBenchmark {
    id: root;
    count: 20;
    staticCount: 1000;
    delegate: Grid {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
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
