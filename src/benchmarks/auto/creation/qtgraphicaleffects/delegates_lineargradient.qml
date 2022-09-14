import QtQuick 2.0
import Qt5Compat.GraphicalEffects
import QmlBench 1.0

// Tests the creation of LinearGradient.
// The values chosen match those in delegates_rect_gradient, so performance can be directly compared
CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 2500;
    delegate: LinearGradient {
        x: QmlBench.getRandom() * (root.width - width)
        y: QmlBench.getRandom() * (root.height - height)
        width: 30
        height: 15
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#ff0000" }
            GradientStop { position: 1.0; color: "#0000ff" }
        }
    }
}

