import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of TextInput
CreationBenchmark {
    id: root;
    count: 20;
    staticCount: 1000;
    delegate: TextInput {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        text: "Qt Quick!"
        font.pixelSize: 10
    }
}
