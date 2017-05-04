# gputhroughput

These benchmarks test your GPU for the most part. The QML part is mostly there
to set things up and the benchmark will fill the graphics pipeline with enough
stuff to make it suffer.

**Note**! These tests are only designed to be used with the sustained-fps shell.

The purpose of these benchmarks is to give an indication how how much one can
expect to put on screen, and to decide how big the screen can actually be.
Especially in the embedded space where a low end chip may have to drive an HD
display, these tests can help to illustrate if the GPU is up to the task or not.

Run them in --fullscreen to get the right numbers. 1 means your in trouble. 2 means
you can manage with a lot of work. 4 and above and you should be good shape, GPU-wise.

