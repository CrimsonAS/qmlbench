import QtQuick 2.0

Item {
    id: root
    width: 320
    height: 480

    Component.onCompleted: {
        var object = benchmark.component.createObject(benchmarkRoot);
        if (!object.hasOwnProperty("count")) {
            print(" - error: property 'count' is lacking from: " + benchmark.input);
            benchmark.abort();
        } else {
            object.anchors.fill = benchmarkRoot;
            root.targetFrameRate = benchmark.screeRefreshRate;
            root.item = object;
            root.item.count = benchmark.count;
            label.updateYerself()
        }
    }

    property Item item;
    property real targetFrameRate;
    property real fps: 0

    Item {
        id: benchmarkRoot
        clip: true
        anchors.fill: parent
    }

    Rectangle {
        id: meter

        clip: true
        width: root.width
        height: 14
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        color: Qt.hsla(0, 0, 1, 0.8);

        Rectangle {
            id: swapTest
            anchors.right: parent.right
            width: parent.height
            height: parent.height
            property real t;
            NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
            property bool inv;
            onTChanged: {
                ++fpsTimer.tick;
                inv = !inv;
            }
            color: inv ? "red" : "blue"
        }

        Timer {
            id: fpsTimer
            running: true;
            repeat: true
            interval: benchmark.fpsInterval
            property var lastFrameTime: new Date();
            property int tick;

            onTriggered: {
                var now = new Date();
                var dt = now.getTime() - lastFrameTime.getTime();
                lastFrameTime = now;
                var fps = (tick * 1000) / dt;
                root.fps = Math.round(fps * 10) / 10;
                tick = 0;

                label.updateYerself();
            }
        }

        Text {
            id: label
            anchors.centerIn: parent
            font.pixelSize: 10

            function updateYerself() {
                var bmName = benchmark.input;
                var lastSlash = bmName.lastIndexOf("/");
                if (lastSlash > 0)
                    bmName = bmName.substr(lastSlash + 1);
                text = bmName + "; FPS=" + root.fps + "; Count=" + item.count;
            }
        }


    }

}
