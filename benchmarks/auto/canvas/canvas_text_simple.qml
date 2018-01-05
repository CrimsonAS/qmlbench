import QtQuick 2.0
import QmlBench 1.0

// Testing the performance of canvas-based text rendering
Item {
    id: root;
    property int count: 100;
    property int staticCount: 1000;

    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 2347; loops: Animation.Infinite }

    Canvas {
        anchors.fill: parent

        property real t: root.t;
        onTChanged: requestPaint();

        onPaint: {
            var ctx = getContext("2d");
            ctx.clearRect(0, 0, width, height);
            var str = "" + Math.floor( t * 1000 ) / 1000;

            for (var i = 0; i < root.count; ++i) {
                ctx.fillText(str, QmlBench.getRandom() * root.width, QmlBench.getRandom() * root.height);
            }
        }
    }
}
