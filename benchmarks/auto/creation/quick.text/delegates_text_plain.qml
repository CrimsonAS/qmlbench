import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of Text with plain formatting forced
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 1000;
    delegate: Text {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        text: "Qt Quick!"
        font.pixelSize: 10
        textFormat: Text.PlainText
    }
}
