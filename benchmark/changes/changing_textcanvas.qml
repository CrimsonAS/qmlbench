import QtQuick 2.0

Item {
    id: root;
    property int count: 100;
    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: 2347; loops: Animation.Infinite }

    Repeater {
        id: repeater
        model: root.count
        Canvas {

            x: Math.floor( Math.random() * root.width )
            y: Math.floor( Math.random() * root.height )

            width: 40
            height: 15

            property real t: root.t;
            onTChanged: requestPaint();

            onPaint: {
                var ctx = getContext("2d");
                ctx.clearRect(0, 0, width, height);
                ctx.fillText("" + Math.floor( t * 1000 ) / 1000, 0, 10);
            }

        }

    }
}
