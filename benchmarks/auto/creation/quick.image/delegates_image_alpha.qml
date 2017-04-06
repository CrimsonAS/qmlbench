import QtQuick 2.0
import QmlBench 1.0

// Test the creation of an Image with an ARGB PNG
// Compare with delegates_image_solid
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;

    delegate: Image {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        source: "../../../../shared/alpha.png"
        width: 20
        height: 20
    }
}
