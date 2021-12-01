import QtQuick 2.4
import QmlBench 1.0

// Tests the creation of TextMetrics with some simple props
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;

    property string fontName: {
        if (Qt.platform.os == "osx") {
            return "Helvetica"
        } else if (Qt.platform.os == "linux") {
            return "Open Sans"
        } else if (Qt.platform.os == "windows") {
            return "Arial"
        }
    }

    delegate: Item {
        TextMetrics {
            font.family: root.fontName
            text: "Hello world"
        }
    }
}



