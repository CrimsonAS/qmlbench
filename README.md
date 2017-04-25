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

## frame-count shell

The frame-count shell constantly performs a set number of operations each frame,
for a given amount of (real world) time, and counts the number of frames the
window is able to generate. If the GL drivers & OS support it, vsync will be
disabled.

The frame-count shell is used for automated benchmark runs, as it is less
susceptible to variance than the sustained-fps shell.

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

The static-count shell is most useful for profiling, as it will repeat the same
operation continuously.

##the qmlbench tool

It comes with a number of default settings which aim to help give stable numbers:

* ``--fps-override``: This one may be very important when using the
  sustained-fps shell. When you ran decidefps,
  it told you a fairly accurate FPS. If QScreen reports something different
  through refreshRate() the calibration won't work and the resulting numbers
  won't mean anything. On OSX, we should be mostly good. On Linux, it is not
  uncommon that the QScreen value is wrong so this option is needed as an
  override to make the tests stable. The tool has not been tested on Windows at
  the time when this is written.
* ``--delay [ms]``: defaults to 2 seconds. An idle time after showing the window
  before starting the benchmark. Especially useful on OSX where the system
  specific to-fullscreen animation takes a while. And that to-fullscreen will
  severly damage the benchmarks.
* ``--help``: Tells you the other options.

## the actual benchmarks

Benchmarks are divided into two types, automated, and manual. The automated
tests are useful for regression testing, and the manual tests are useful for
determining the capabilities of a new piece of hardware.

To run all the automated tests, simply run this, and go have lunch (it is not a
fast process):

> ./src/qmlbench benchmarks/auto/

You can also run a subset of them by providing that directory instead:

> ./src/qmlbench benchmarks/auto/creation/quick.item/

Or even individual tests, by passing the filenames directly.

### benchmarks/auto/creation/

This is a suite of tests to find out how good we are with creation, rendering
and destruction of objects. Some of the tests are also written in such a way
that they can be compared between each other -- these are usually noted in the
descriptive comment at the top of the test.

Creation is an important factor: our items should be light, as creating a dialog
or page of UI can often creating a few hundred different items (especially Text,
Item, Rectangle, etc). In addition to this, there's code outside of our control
on the application end: JS logic, model interaction, database or file I/O all
comes in addition - so we need to leave plenty of performance left over for the
end user.

### benchmarks/manual/gputhroughput

These benchmarks test your GPU for the most part. The QML part is mostly there
to set things up and the benchmark will fill the graphics pipeline with enough
stuff to make it suffer.

**Note**! These tests are only designed to be used with the sustained-fps shell.

The purpose of these benchmarks is to give an indication how how much one can
expect to put on screen, and to decide how big the screen can actually be.
Especially in the embedded space where a low end chip may have to drive an HD
display, these tests can help to illustrate if the GPU is up to the task or not.

* ``blendedrect.qml``: Stacks x number of alphablended rectangles on top of
  each other. Rough test of fillrate.
* ``blendedtexture.qml``: Stacks x number of alphablended textures on top of
  each other. Rouch test of fill and texel rate.
* ``opaquerect.qml``: Stacks x number of opaque rectangles on top of each other.
  Will go a lot higher than blendedrect if the target hardware supprts
  [early-z](https://en.wikipedia.org/?title=Z-buffering)
* ``opaquetexture.qml``: Stacks x number of opaque textures on top of each other.
  Will go a lot higher than blendedtexture if target hardware supports
  [early-z](https://en.wikipedia.org/?title=Z-buffering)

The numbers in these benchmarks mean how many times you can fill the screen. Run
them in --fullscreen to get the right numbers. 1 means your in trouble. 2 means
you can manage with a lot of work. 4 and above and you should be good shape, GPU-wise.

* ``drawcalls.qml``: Take this one with a grain of salt. Graphics drivers have a
  lot of overhead in how drawing is set up, and seeing an individual GL call take
  up to a millisecond (yes, a millisecond) is not uncommon. This test is a highly
  constructed case to try to get a rough ballpart of how many discrete draw calls
  the GL stack is capable of.

  This is mostly important if you end up with an application that fails to do
  batching in the scene graph renderer, but as this situation will typically
  have many other performance problems, this may not be a useful benchmark for
  the most part.
* ``gaussblur.qml``: This benchmark is added to test how feasible live blurring
  is. Live blurring is an extreme fillrate test and is only something that
  should be considered on gaming and industrial hardware. And then you still
  probably want to cheat :p

### benchmarks/auto/animations/comparison/

These benchmarks compare a number of different ways of moving images around, and
are help validate the casual gaming idea which should be very viable with QML.

The benchmarks give an indication of how many animated items can run
simultaneously in the UI. It should be in the thousands.

One quirk if you run these is that on a threaded renderloop, the animation one
runs faster than animators. This is because the work is broken into two major
chunks. One is doing the animation while the other is doing the batching in the
renderer and scheduling the rendering. If those happen on separate threads, we
get better parallelization so 'animation' comes out better.

However, if you try again with QSG_RENDER_LOOP=windows in the environment,
you'll see that if it all happens on the same thread, then animators are a bit
cheaper (because they are simpler).

### benchmarks/auto/changes

These benchmarks help measure the impact of various types of changes in a scene
(for instance, changing one text item out of many, changing all text items at
once).

