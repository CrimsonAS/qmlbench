import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of the JS vector4d type as a property.
CreationBenchmark {
    id: root;
    count: 50
    staticCount: 2500
    delegate: Item {
        property vector4d value: Qt.vector4d(1, 2, 3, 4)
    }
}

