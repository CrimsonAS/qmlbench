import QtQuick 2.2
import QmlBench 1.0

// Move images around using a global property binding, to be compared with a
// number of other similar ways to move them around.
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
            x: QmlBench.getRandom() * (root.width - width)
            y: QmlBench.getRandom() * (root.height - height)
            width: 40
            height: 40
            rotation: index + frameCount
        }
    }
}

