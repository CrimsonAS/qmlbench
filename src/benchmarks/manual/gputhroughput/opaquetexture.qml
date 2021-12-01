import QtQuick 2.0

// Stacks x number of opaque textures on top of each other.
// Will go a lot higher than blendedtexture if target hardware supports
// [early-z](https://en.wikipedia.org/?title=Z-buffering)
Item {
    id: root;

    property int count;
    property int staticCount: 0

    Repeater {
        model: root.count;
        Rectangle {
            width: root.width
            height: root.height
            color: Qt.hsla((index * .271) % 1.0, 0.5, 0.5);
            z: index
            layer.enabled: true
            layer.effect: ShaderEffect {
                blending: false
            }
        }
    }


}
