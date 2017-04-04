import QtQuick 2.0
import QmlBench 1.0

CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 1000;
    delegate: Text {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        text: "गोपनीयता लाभान्वित"
        font.pixelSize: 10
    }
}


