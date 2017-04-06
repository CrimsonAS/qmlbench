import QtQuick 2.0
import QmlBench 1.0

// Test the creation of an antialiased image.
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;

    property var names: [
        "butterfly-wide.png",
        "butterfly-half.png",
        "butterfly-collapsed.png"
    ];

    delegate: Image {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        source: "../../../../shared/" + root.names[index % 3];
        width: 20
        height: 20
        antialiasing: true
    }
}

