[<-- Back to README.md](../README.md)

# Creating custom benchmarks
Sometimes an existing test doesn't cover your hacks to qt, or maybe you're adding new functionality,
either way, creating new benchmarks is easy. ~Here's how!~

## The fundamentals of QMLBench
When running qmlbench in `frame-count shell` (default mode), a static count of operations is
performed for each frame rendered. QMLbench renders as quickly as possible over a 20 second
(default) period counting the total frames rendered during that period. This is the 'score'.
In this way, a benchmark should be defined to perform as many operations as possible without
hanging qmlbench, typically managing to run at a few frames-per-second on mid-grade hardware.
If this rule is observed, a benchmark should be able to run on low-grade embedded hardware without
timing out and hanging, as well as on high-end hardware without acheiving 60fps
(the typical soft cap if vsync cannot be disabled).


## A sample file

    import QtQuick 2.0
    import QmlBench 1.0

    // Tests the creation of rectangles with *NO* alpha channel.
    // Compare with delegates_rect_blended & delegates_rect_radius
    CreationBenchmark {
        id: root;
        count: 50;
        staticCount: 2500;
        delegate: Rectangle {
            x: QmlBench.getRandom() * (root.width - width)
            y: QmlBench.getRandom() * (root.height - height)
            width: 30
            height: 15
            color: "steelblue"
        }
    }


## The root level declarations
In the example above, we're testing the creation speed of the `Rectangle` delegate, so we're using `CreationBenchmark`.

There are three types of root level declarations allowed:
- Benchmark
    - The basic QMLBench type.
    - Good for testing performance of operations with fixed durations, such as animations.
    - Provides the `count` and `staticCount` properties.
    - Provides a tick property `t` from 0 to 1, lasting 1000ms that benchmarks can access.
- CreationBenchmark
    - Inherits Benchmark
    - Creates `count` instances of an object before displaying the rendered frame.
    - Destroys all items and recreates them each frame.
- Item (Inherited from QtBase)
    - must declare the following manually
    ```
    id: root;
    property int count: your_count_here;
    property int staticCount: your_static_count_here;

    property real t;
    NumberAnimation on t { from: 0; to: 1; duration: your_ticker_duration_in_ms; loops: Animation.Infinite }
    ```


## Count and StaticCount
In general, it is good practice to set both values so the benchmark can be used with any shell type.

### count
Used in `sustained-fps shell` mode. The benchmark will alter this value trying to achieve 60fps
(or rather, the screen's refresh rate). Set this value to as a starting point for the benchmark to
try first.

### staticCount
Used in all `static` shell types. This is the count of objects to create on startup.
- If using CreationBenchmark root type, this is the count of objects to create each frame.
- If using Benchmark, this is the count of objects to create at the start of the benchmark run.


## Item delegates (Creation Benchmarks)
In the code sample above, the `delegate` (a property of CreationBenchmark) is defined as a
Rectangle with a fixed size and random position, shaded a single color with no transparency.
- This `delegate` will be repeated in the render window in accordance with the properties `count`
or `staticCount`, for a given shell type.
- The delegates will be destroyed and regenerated for each frame rendered.


## Repeaters (Non-creation benchmarks)
Let's take a look at a new code sample. For this benchmark, we're repeating an operation `count`
times using `Repeater` at the benchmark outset, and continuing to render for the duration of the test.

    import QtQuick 2.2
    import QmlBench 1.0

    // Move images around using Animation types, to be compared with a number of
    // other similar ways to move them around.
    Benchmark {
        id: root;

        count: 500
        staticCount: 20000

        Repeater {
            model: root.count
            Image {
                source: "../../../../shared/butterfly-wide.png"
                x: QmlBench.getRandom() * (root.width - width)
                y: QmlBench.getRandom() * (root.height - height)
                width: 40
                height: 40

                SequentialAnimation on rotation {
                    NumberAnimation { from: -10; to: 10; duration: 500; easing.type: Easing.InOutCubic }
                    NumberAnimation { from: 10; to: -10; duration: 500; easing.type: Easing.InOutCubic }
                    loops: Animation.Infinite
                }
            }
        }
    }

In this case, the image `butterfly-wide.png` is being randomly placed in the render window, and has
an animation applied on `rotation`, looping infinitely. The image and animation will be repeated 500
times, resulting in 500 randomly placed, rotating images for the duration of the benchmark run.

- Multiple `Repeaters` can be added if necessary, though it's recommended to test the smallest
possible operation specific to your change.
