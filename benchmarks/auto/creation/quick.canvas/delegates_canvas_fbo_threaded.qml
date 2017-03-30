import QtQuick 2.0
import QmlBench 1.0

// Testing the performance of creating a canvas
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Canvas {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
        renderTarget: Canvas.FramebufferObject
        renderStrategy: Canvas.Threaded
        // *just* measuring canvas creation. nothing else.
    }
}

