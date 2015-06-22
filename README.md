#qmlbench

qmlbench provides a set of simple benchmarks which aim to find how many times a certain operation can be performed per frame while still sustaining a perfect velvet framerate.

You can also view it as a test of: *How much stuff can I do before things start to fall apart.*

The benchmarks excersice a very large part of the Qt Quick / QML / Qt Gui / Qt Core stack and can in many ways be considered a decent metric for overall Qt performance.

##before you start

As already mentioned, the goal is to find how much is possible while hitting a perfect framerate. So the first goal is to verify that your system is capable of hitting a perfect framerate at all. This is called the swap test. Test it by running:

> ./qmlbench --decide-fps

and observe. If you see a pulsating purple rectangle you are in good shape. If you see flashes of red and/or blue or if the purple rectangle has horizontal regions of red and blue, your system is not able to hit a perfect framerate. 

Fix that first.

The second thing one needs to do is to kill absolutely every running thing on the computer. Webbrowser typically suck a lot of juice at random points in time. Same with mail clients, irc clients, media players, etc. When you benchmark, run only your terminal and the benchmarking application. Disable system wide file indexing in your OS, etc.. Any background process that might pop up will cause instability in the results. 

##the qmlbench tool

It comes with a number of default settings which aim to help give stable numbers:

- --fps-override: This one is potentially very important. When you ran qmlbench with --decide-fps, it told you a fairly accurate FPS. If QScreen reports something different through refreshRate() the calibration won't work and the resulting numbers won't mean anything. On OSX, we should be mostly good. On Linux, it is not uncommon that the QScreen value is wrong so this option is needed as an override to make the tests stable. The tool has not been tested on Windows at the time when this is written.
- --delay [ms]: defaults to 2 seconds. An idle time after showing the window before starting the benchmark. Especially useful on OSX where the system specific to-fullscreen animation takes a while. And that to-fullscreen will severly damage the benchmarks
- --help: Tells you the other options..

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
