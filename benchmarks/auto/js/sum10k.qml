import QtQuick 2.0
import QmlBench 1.0

CreationBenchmark {
    id: root;
    count: 50;
    staticCount: 1000;
    delegate: Item {
        x: sum(10000);
    }

    function sum(n) {
        var x = 0;
        for (var i=0; i<n; ++i)
            x = x + x;
        return x;
    }
}
