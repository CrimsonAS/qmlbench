import QtQuick 2.0
import QmlBench 1.0
import QtQuick.Controls 2.0

// Tests the creation of QQC2's MenuItem type.
CreationBenchmark {
    id: root
    count: 20
    staticCount: 1000
    delegate: MenuItem {
        x: Math.random() * root.width - width
        y: Math.random() * root.height - height
        text: "MenuItem"
        checkable: index % 2
        checked: index % 2
        down: index % 3
    }
}
