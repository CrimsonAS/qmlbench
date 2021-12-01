import QtQuick 2.0
import QmlBench 1.0

// Tests access of rect properties from a C++ model
Benchmark {
    id: root;
    count: 50
    staticCount: 2500

    onTChanged: {
        testModel.rowNumber = 0
        testModel.rowNumber = root.count
    }

    Repeater {
        model: TestModel {
            id: testModel
        }
        delegate: Item {
            property rect value: model.rectRole
        }
    }
}

