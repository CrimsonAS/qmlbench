import QtQuick 2.2
import QmlBench 1.0

Benchmark {
    id: root;

    count: 500
    staticCount: 20000

    property int frameCount
    onTChanged: {
        frameCount++
    }

    Repeater {
        model: root.count
        Image {
            source: "../../../../shared/butterfly-wide.png"
            x: Math.random() * (root.width - width)
            y: Math.random() * (root.height - height)
            width: 40
            height: 40
            rotation: index + frameCount
        }
    }
}

