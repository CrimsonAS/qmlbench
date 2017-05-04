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

