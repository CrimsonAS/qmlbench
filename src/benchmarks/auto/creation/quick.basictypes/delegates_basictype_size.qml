import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of the JS size type as a property.
CreationBenchmark {
    id: root;
    count: 50
    staticCount: 2500
    delegate: Item {
        property size value: Qt.size(0, 150)
    }
}

