import QtQuick 2.0

Item {
    id: root
    width: 320
    height: 480

    Component.onCompleted: {
        var object = benchmark.component.createObject(benchmarkRoot);
        if (!object.hasOwnProperty("count")) {
            print(" - error: input is lacking from: " + benchmark.input);
            benchmark.abort();
        } else {
            object.anchors.fill = benchmarkRoot;
            root.targetFrameRate = benchmark.screeRefreshRate;
            root.item = object;
            label.updateYerself()
        }
    }

    property Item item;
    property real targetFrameRate;
    property int stableCount;
    property int knownGood: 0;
    property int knownBad: -1;
    property real fps: 0

    Item {
        id: benchmarkRoot
        clip: true
        anchors.fill: parent
    }

    Rectangle {
        id: meter

        clip: true
        width: root.width
        height: 14
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        color: Qt.hsla(0, 0, 1, 0.8);

        Rectangle {
            id: swapTest
            anchors.right: parent.right
            width: parent.height
            height: parent.height
            property real t;
            NumberAnimation on t { from: 0; to: 1; duration: 1000; loops: Animation.Infinite }
            property bool inv;
            onTChanged: {
                ++fpsTimer.tick;
                inv = !inv;
            }
            color: inv ? "red" : "blue"
        }

        Connections {
            target: benchmark.view
            onFrameSwapped: fpsTimer.frameSwapped()
        }


        Timer {
            id: fpsTimer
            running: false;
            repeat: true
            interval: benchmark.fpsInterval
            property var lastFrameTime: new Date();
            property int tick;

            /* Start over is used to give us a few frames of wiggleroom
               after changing count to get back to good running speed.
               Many benchmarks will have a high load when changing count
               to set up the scene, but as the point of the test is to measure
               how the scene fares when count is a certain value, we need to skip
               those to get stable results. Hence we skip 2 frames. The first frame
               is because we might be in middle of a render pass (due to threaded rendering)
               right now and the second is the one that contained the change. After two
               frames, we are certain that any lingering effect of changing count has
               been normalized.
             */
            property int swapCountDown: 5;
            function startOver() {
                stop();
                swapCountDown = 5;
            }
            function frameSwapped() {
                if (!running && --swapCountDown < 0) {
                    tick = 0;
                    lastFrameTime = new Date();
                    start();
                }
            }


            onTriggered: {
                var now = new Date();
                var dt = now.getTime() - lastFrameTime.getTime();
                lastFrameTime = now;
                var fps = (tick * 1000) / dt;
                root.fps = Math.round(fps * 10) / 10;
                tick = 0;

                /* The logic for caluclating a good count is that we find good values
                   which allow us to run at 60 fps and bad values, where we fail to
                   run at 60 fps. bad is unknown so we double the count until it is found.
                   Then we binary search between good and bad until we have a solid number.

                   The error ratio and interval for checking can be tweaked by the benchmark
                   tool to improve the accuracy of the results.
                 */
                var errorRatio = Math.abs(1 - root.fps / root.targetFrameRate);
                var ok = errorRatio < benchmark.fpsTolerance

                var max = Number.MAX_VALUE;
                if (item.hasOwnProperty("maxCount"))
                    max = item.maxCount;

                if ((knownBad > 0 && Math.abs(knownGood - knownBad) < 2)
                        || item.count >= max) {
                    fpsTimer.stop();
                    benchmark.recordOperationsPerFrame(ok ? item.count : knownGood);
                    return;
                }

                if (benchmark.verbose) {
                    print(" --- count: " + item.count + ", " +
                          "Good: " + (ok ? item.count : knownGood) + ", " +
                          "Bad: " + (!ok ? item.count : knownBad) + ", " +
                          (ok ? "Success" : "Fail") + ", " +
                          "Fps: " + root.fps);
                }


                if (ok) {
                    knownGood = item.count;
                    var incr = Math.max(1, item.count * 2);
                    if (knownBad > 0)
                        incr = (knownBad - knownGood) / 2.0;
                    item.count = Math.min(max, item.count + incr);
                    startOver();
                } else {
                    knownBad = item.count;
                    var decr = (knownBad - knownGood) / 2.0
                    item.count -= decr;
                    startOver();
                }

                label.updateYerself();
            }
        }

        Text {
            id: label
            anchors.centerIn: parent
            font.pixelSize: 10

            function updateYerself() {
                var bmName = benchmark.input;
                var lastSlash = bmName.lastIndexOf("/");
                if (lastSlash > 0)
                    bmName = bmName.substr(lastSlash + 1);
                text = "ops/frame: " + item.count + " - " + bmName;
            }
        }


    }

}
