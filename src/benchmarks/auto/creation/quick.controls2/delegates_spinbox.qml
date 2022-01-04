import QtQuick 2.0
import QmlBench 1.0
import QtQuick.Controls 2.0

// Tests the creation of QQC2's SpinBox type.
CreationBenchmark {
    id: root
    count: 20
    staticCount: 250
    delegate: SpinBox {
        x: QmlBench.getRandom() * root.width - width
        y: QmlBench.getRandom() * root.height - height
        value: index / root.staticCount * to
    }
}
