import QtQuick 2.0
import QmlBench 1.0
import QtQuick.Layouts 1.0

// Tests the creation of GridLayout, to be compared with Grid
CreationBenchmark {
    id: root;
    count: 20;
    staticCount: 1000;
    delegate: GridLayout {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        columns: 2
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
