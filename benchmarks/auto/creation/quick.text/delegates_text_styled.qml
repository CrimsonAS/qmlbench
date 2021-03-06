import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of Text with styled formatting forced
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 1000;
    delegate: Text {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        text: "Qt Quick!"
        font.pixelSize: 10
        textFormat: Text.StyledText
    }
}
