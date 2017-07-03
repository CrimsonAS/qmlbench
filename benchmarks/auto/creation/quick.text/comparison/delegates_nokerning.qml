import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of Text with kerning disabled.
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 1000;

    delegate: Text {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        text: "OATS FLAVOUR WAY"
        font.family: "Times New Roman"
        font.kerning: false
        font.pixelSize: 10
    }
}
