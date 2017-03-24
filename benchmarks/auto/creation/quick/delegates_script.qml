import QtQuick 2.0
import QmlBench 1.0

// TODO: what is this testing? looks like it may be attempting to test the cost
// of a script handler, maybe, but perhaps we should simplify and use an Item
// with an x/y to take Rectangle/color out of the mix?
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Rectangle {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
        color: "steelblue"
        Component.onCompleted: {
            var t = Math.random();
            color = Qt.hsla(t, 1, 0.5);
        }
    }
}
