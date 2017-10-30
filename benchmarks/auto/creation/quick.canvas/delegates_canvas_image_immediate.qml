import QtQuick 2.0
import QmlBench 1.0

// Testing the performance of creating a canvas
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Canvas {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        width: 30
        height: 15
        renderTarget: Canvas.Image
        renderStrategy: Canvas.Immediate
        // *just* measuring canvas creation. nothing else.
    }
}

