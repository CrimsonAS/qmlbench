import QtQuick 2.0

Item {
    id: root;
    property int count: 20;

    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
    onTChanged: {
        repeater.model = 0;
        repeater.model = root.count
    }

    Component.onCompleted: repeater.model = root.count

    Repeater {
        id: repeater
        Text {
            x: Math.random() * (root.width - width)
            y: Math.random() * (root.height - height)
            width: root.width * 0.33
            wrapMode: Text.WordWrap
            font.pixelSize: 10
            text: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc in volutpat nibh, ut convallis neque. Nam placerat tellus et ante feugiat condimentum. Nulla facilisi. Sed in lobortis magna. Duis eu mattis ante. Donec pulvinar ante est, ac ultricies elit commodo vitae. Sed ut luctus odio, quis aliquet metus. Curabitur quis lacus fringilla, luctus justo id, volutpat dui. Aliquam id nunc gravida, pharetra augue ac, tristique nibh."
        }
    }
}
