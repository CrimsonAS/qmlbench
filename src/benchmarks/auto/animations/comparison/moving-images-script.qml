import QtQuick 2.2
import QmlBench 1.0

// Move images around using a property binding, to be compared with a number of
// other similar ways to move them around.
Benchmark {
    id: root;

    count: 500
    staticCount: 20000

    Repeater {
        model: root.count
        Image {
            source: "qrc:///shared/butterfly-wide.png"
            x: QmlBench.getRandom() * (root.width - width)
            y: QmlBench.getRandom() * (root.height - height)
            width: 40
            height: 40

            property real t;
            rotation: 10 * Math.sin(t * Math.PI * 2 + Math.PI);

            SequentialAnimation on t {
                NumberAnimation { from: 0; to: 1; duration: 1000; }
                loops: Animation.Infinite
            }
        }
    }
}
