# v8bench

This is a set of simple wrapper tests that invoke a particular test suite from
the well known v8-bench test suite, that test different aspects of JavaScript
performance.

This uses qmlbench (rather than v8-bench.js) as the test harness in order to
integrate with the rest of the tests, and perhaps also to give a more stable
result (by running for a longer duration, and running repeatedly, as all
qmlbench tests do).
