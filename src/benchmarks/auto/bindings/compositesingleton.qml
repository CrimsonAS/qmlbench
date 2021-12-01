import QtQuick 2.0
import QmlBench 1.0
import "qrc:///shared"

// Tests the creation of Items bound to a QML-defined singleton
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Item {
        x: Globals.realProp
        y: Globals.intProp
        smooth: Globals.boolProp
    }
}
