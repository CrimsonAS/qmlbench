import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of IntValidator
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Item {
        property IntValidator validator: IntValidator {
            top: 100
            bottom: 0
        }
    }
}

