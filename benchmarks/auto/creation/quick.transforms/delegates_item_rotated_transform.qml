import QtQuick 2.0
import QmlBench 1.0

// Rotate an Item.
// Compare with delegates_item_rotated_transform.
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: Item {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
        transform: Rotation {
            angle: 45
        }
    }
}


