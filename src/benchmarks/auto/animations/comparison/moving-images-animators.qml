import QtQuick 2.2
import QmlBench 1.0

// Move images around using Animator types, to be compared with a number of
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

            SequentialAnimation on rotation {
                RotationAnimator { from: -10; to: 10; duration: 500; easing.type: Easing.InOutCubic }
                RotationAnimator { from: 10; to: -10; duration: 500; easing.type: Easing.InOutCubic }
                loops: Animation.Infinite
            }
        }
    }
}
