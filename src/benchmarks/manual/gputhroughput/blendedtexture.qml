import QtQuick 2.0

// Stacks x number of alphablended textures on top of each other.
// Rouch test of fill and texel rate.
Item {
    id: root;
    property int count: 8;
    property int staticCount: 0

    Repeater {
        model: root.count;
        Rectangle {
            width: root.width
            height: root.height
            color: Qt.hsla((index * .271) % 1.0, 0.5, 0.5, 0.5);
            layer.enabled: true
            layer.format: ShaderEffectSource.RGBA
        }
    }
}
