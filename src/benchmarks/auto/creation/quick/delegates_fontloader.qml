import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of FontLoader with some simple props
// ### add a test for FontLoader from TTF?
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;

    delegate: Item {
        FontLoader {
        }
    }
}

