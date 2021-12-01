import QtQuick 2.0
import QmlBench 1.0

// Test the creation of an Image with an RGB PNG
// Compare with delegates_image_alpha
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;

    delegate: Image {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        source: "qrc:///shared/solid.png"
        width: 20
        height: 20
    }
}
