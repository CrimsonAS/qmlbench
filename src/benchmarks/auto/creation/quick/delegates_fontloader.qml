import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of FontLoader with some simple props
// ### add a test for FontLoader from TTF?
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
        FontLoader {
            name: root.fontName
        }
    }
}

