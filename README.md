# qmlbench

qmlbench is a tool for benchmarking Qt, QML and QtQuick as a whole stack rather
than in isolation. The benchmarks it provides exercise a very large part of
Quick, QML, Gui, Core, and as a result can be considered a decent metric for
overall Qt performance.

As with any benchmarking, if you want reliable results, the first thing you must
attend to is ensuring that the system under test is doing the absolute minimum
possible. "Typical" operating systems (OS X, Ubuntu, ...) all have a lot of
"background noise" in the form of file indexing, automatic updates. All of this
should be disabled if possible. You may want to consider disabling networking
for the duration of the benchmarking run to help keep things "quiet".

In terms of running applications, you should have the bare minimum if at all
possible. No email clients, no IRC, no media players. Only your terminal, and
qmlbench should be running. If possible (e.g. on Linux), you may want to even
look into setting up a custom desktop environment session that doesn't launch
anything else (so no gnome-session etc).

Remember: Any background process that might pop up will cause instability in
the results!

# shells

qmlbench provides a number of "shells", which are the contaners for the
benchmarking logic. Different shells work in different ways, and might measure
different things.

## sustained-fps shell

The sustained-fps shell provides a measure of how many times a certain operation
can be performed per frame while sustaining a velvet smooth framerate matching the
host's screen refresh rate.

Summarized, it can be looked at as a test of:
*How much stuff can I do before things start to fall apart.*.

Note that the sustained-fps shell _requires_ a stable frame rate (see the
prerequisites section), which is often hard for systems to provide, so this
shell may not be a good choice for you, and may not be appropriate for automated
regression testing.

### prerequisites

As already mentioned, the goal of the sustained-fps shell is to find how much is
possible while hitting a perfect framerate. So the first goal is to verify
that your system is capable of hitting a perfect framerate at all. This is
called the swap test. Test it by running the following, and observing the screen:

> ./tools/decidefps/decidefps

If you see a pulsating purple rectangle you are in good shape. If you see
flashes of red and/or blue or if the purple rectangle has horizontal regions
of red and blue, your system is not able to hit a perfect framerate.

Fix that first, or you will not get stable/sensible results from the
sustained-fps shell.

Lastly, check if your system is running with the 'basic' render loop
(QSG_INFO=1 in the environment will tell you). If you are, you will most likely
not be able to get velvet FPS because the animations are timer driven and will
skip once or twice per frame by default. If threaded is not an option for
whatever reason, try with QSG_RENDER_LOOP=windows (which uses vsync driven
animations).

## static-count shell

The static-count shell constantly performs a set number of operations (the
staticCount property in benchmarks) each frame. It is useful for profiling.

You can override the staticCount value by passing a `--count N` command line
argument, where N is the staticCount you want to use instead.

## frame-count shell

The frame-count shell constantly performs a set number of operations each frame,
for a given amount of (real world) time, and counts the number of frames the
window is able to generate. If the GL drivers & OS support it, vsync will be
disabled.

##the qmlbench tool

It comes with a number of default settings which aim to help give stable numbers:

- --fps-override: This one is potentially very important. When you ran decidefps,
it told you a fairly accurate FPS. If QScreen reports something different through refreshRate() the calibration won't work and the resulting numbers won't mean anything. On OSX, we should be mostly good. On Linux, it is not uncommon that the QScreen value is wrong so this option is needed as an override to make the tests stable. The tool has not been tested on Windows at the time when this is written.
- --delay [ms]: defaults to 2 seconds. An idle time after showing the window before starting the benchmark. Especially useful on OSX where the system specific to-fullscreen animation takes a while. And that to-fullscreen will severly damage the benchmarks
- --help: Tells you the other options..

TODO describe --hardware-multiplier, --count...

##the actual benchmarks

### benchmarks/creation

This is a small suite of different creation tests. The idea is to find out how good we are with creation and destruction of objects. Both how good we are overall, and which items need more work than others. As we would like items to be light, these numbers should be high. Creating a dialog page easily involves creating 2-300 Item objects, 100 Rectangle objects, 200 Text elements, etc. And any bindings, js logic, model interaction, sql database fetching (threaded of course) comes in addition. For the items, we should strive for 1000 ops / frame on desktop machines and 100 ops / frame on medium to low end embedded. 

- delegates_qobject.qml: Tests raw QObject creation, this is without any of the QML / QQuickItem overhead so these should be a lot higher than anything else.
- delegates_item.qml: Tests raw QQuickItem creation. 

These two tests are non-visual, so the addition from the graphics stack is mostly just a the swap (which can take quite a bit of time on its own, but that is no fault of Qt).

- delegates_rect.qml
- delegates_text.qml
- delegates_image.qml

And others.. Test the basic built-in items

Just run:

> ./qmlbench benchmarks/creation

to run and test them all. As all tests are run 5 times and the results averaged, it will take a while :)


### benchmarks/gputhroughput

These benchmarks test your GPU for the most part. The QML part is mostly there to set things up and the benchmark will fill the graphics pipeline with enough stuff to make it suffer. The purpose of these benchmarks is to give an indication how how much one can expect to put on screen, and to decide how big the screen can actually be. Especially in the embedded space where a low end chip may have to drive an HD display, these tests can help to illustrate if the GPU is up to the task or not.

- blendedrect.qml: Stacks x number of alphablended rectangles on top of each other. Rough test of fillrate. 
- blendedtexture.qml: Stacks x number of alphablended textures on top of each other. Rouch test of fill and texel rate.  
- opaquerect.qml: Stacks x number of opaque rectangles on top of each other. Will go a lot higher than blendedrect if the target hardware supprts early-z (https://en.wikipedia.org/?title=Z-buffering)
- opaquetexture.qml: Stacks x number of opaque textures on top of each other. Will go a lot higher than blendedtexture if target hardware supports early-z.

The numbers in these benchmarks mean how many times you can fill the screen. Run them in --fullscreen to get the right numbers. 1 means your in trouble. 2 means you can manage with a lot of work. 4 and above and you should be good shape, GPU-wise.

- drawcalls.qml: Take this one with a grain of salt. Graphics drivers have a lot of overhead in how drawing is set up, and seeing an individual GL call take up to a millisecond (yes, a millisecond) is not uncommon. This test is a highly constructed case to try to pinpoint the rough ballpart of how many discrete draw calls the GL stack is capable of. This is mostly important if you end up with an application that fails to do batching in the scene graph renderer, but then you will have loads of other performance problems as well, so perhaps just ignore this benchmark all together. 
- gaussblur.qml: This benchmark is added to test how feasible live blurring is. Live blurring is an extreme fillrate test and is only something that should be considered on gaming and industrial hardware. And then you still probably want to cheat :p

### benchmarks/images

These benchmarks are there to validate the casual gaming idea which should be very viable with QML. 

The following three benchmarks give an indication of how many animated items can run simultaneously in the UI. It should be in the thousands. 

- moving-images-animation.qml
- moving-images-animators.qml
- moving-images-script.qml

One quirk if you run these is that on a threaded renderloop, the animation one runs faster than animators. This is because the work is broken into two major chunks. One is doing the animation while the other is doing the batching in the renderer and scheduling the rendering. If those happen on separate threads, we get better parallelization so 'animation' comes out better. However, if you try again with QSG_RENDER_LOOP=windows in the environment, you'll see that if it all happens on the same thread, then animators are a bit cheaper (because they are simpler)

### benchmarks/changes

These benchmarks were created as a result of some painful bugreports we got about text performance. So we're testing how different types of changes in text affect the rest of the scene rendering. It is a very specific benchmark, and could be ignored by anyone which is not directly investigating scene graph internals of text drawing.

## what's missing?

I'd love to get creating benchmarks and some page-stack transition benchmarks from Qt Quick Controls 1.0 and the upcoming 2.0 in here.
