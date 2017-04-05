import QtQuick 2.0
import QmlBench 1.0

// Tests the creation of ShaderEffectSource with a few simple props.
CreationBenchmark {
    id: root;
    count: 50
    staticCount: 500
    delegate: ShaderEffectSource {
        x: Math.random() * (root.width - width)
        y: Math.random() * (root.height - height)
        width: 30
        height: 15
        sourceItem: sourceRectItem
    }

    Rectangle {
        id: sourceRectItem
        color: "#ff0000"
        width: 30
        height: 15
    }
}

