import QtQuick 2.0
import QmlBench 1.0

CreationBenchmark {
    id: root;
    count: 20;
    staticCount: 1000;
    delegate: TextInput {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        text: "Qt Quick!"
        font.pixelSize: 10
    }
}
