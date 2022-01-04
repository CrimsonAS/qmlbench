import QtQuick 2.0
import QmlBench 1.0
import QtQuick.Controls 2.0

// Tests the creation of QQC2's TabBar type.
CreationBenchmark {
    id: root
    count: 20
    staticCount: 250
    delegate: TabBar {
        x: QmlBench.getRandom() * root.width - width
        y: QmlBench.getRandom() * root.height - height
        currentIndex: index / root.staticCount * count
        TabButton {
            text: "Tab1"
        }
        TabButton {
            text: "Tab2"
        }
        TabButton {
            text: "Tab3"
        }
    }
}
