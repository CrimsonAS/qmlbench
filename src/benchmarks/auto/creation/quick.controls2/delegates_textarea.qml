import QtQuick 2.0
import QmlBench 1.0
import QtQuick.Controls 2.0

// Tests the creation of QQC2's TextArea type.
CreationBenchmark {
    id: root
    count: 20
    staticCount: 500
    delegate: TextArea {
        x: QmlBench.getRandom() * root.width - width
        y: QmlBench.getRandom() * root.height - height
        text: "Text\nArea"
    }
}
