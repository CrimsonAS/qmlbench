import QtQuick 2.0
import QmlBench 1.0

CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Item {
        property DoubleValidator validator: DoubleValidator {
            top: 100
            bottom: 0
            decimals: 2
        }
    }
}

