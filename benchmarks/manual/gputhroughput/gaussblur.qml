import QtQuick 2.2
import QtGraphicalEffects 1.0

// This benchmark is added to test how feasible live blurring
// is. Live blurring is an extreme fillrate test and is only something that
// should be considered on gaming and industrial hardware. And then you still
// probably want to cheat :p
Item {
    id: root;
    property int count: 8
    property int maxCount: 32;
    property int staticCount: 0

    width: 600
    height: 600

    Image {
        id: contentRoot
        anchors.fill: parent
        fillMode: Image.PreserveAspectCrop
        source: "grapes.jpg"
        Rectangle {
            color: "palegreen"
            border.color: "black"
            width: parent.width / 3
            height: parent.width / 3
            RotationAnimator on rotation { from: 0; to: 360; duration: 10000; loops: Animation.Infinite }
            anchors.centerIn: parent
            antialiasing: true
        }

        layer.enabled: true
        layer.effect: GaussianBlur {
            samples: root.count
            radius: root.count
            deviation: Math.sqrt(root.count)
        }
    }
}
