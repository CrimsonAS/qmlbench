# comparison

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
