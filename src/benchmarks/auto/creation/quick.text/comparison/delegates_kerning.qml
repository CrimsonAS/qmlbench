import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of Text with no special flags set.
// Used as baseline for delegates_nokerning and delegates_shaping
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 1000;
    delegate: Text {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        text: "OATS FLAVOUR WAY"
        font.family: "Times New Roman"
        font.pixelSize: 10
    }
}
