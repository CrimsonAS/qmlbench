import QtQuick 2.0
import QmlBench 1.0
import QtQuick.Controls 2.0

// Tests the creation of QQC2's TabBar type.
CreationBenchmark {
    id: root
    count: 20
    staticCount: 250
    delegate: TabBar {
        x: Math.random() * root.width - width
        y: Math.random() * root.height - height
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
