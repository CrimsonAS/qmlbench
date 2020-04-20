import QtQuick 2.2
import QmlBench 1.0
import QtQuick.Layouts 1.3

Benchmark {
    id: root;
    count: 50;
    staticCount: 1000;

    // For each frame, change each child item of the layout:
    //      * Layout.rowSpan
    //      * Layout.columnSpan

    property int frameCounter : 0
    onTChanged: {
        frameCounter++
    }
    GridLayout {
        columns: 100
        Repeater {
            id: repeater
            model: root.count
            Rectangle {
                color: Qt.hsla((index / 50) % 1, 1.0, 0.8)
                Layout.columnSpan: (index * 3) % 17 + 1 + frameCounter % 10
                Layout.rowSpan: (index * 5) % 17 + 1 + frameCounter % 10
                implicitWidth: 0
                implicitHeight: 0
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}


