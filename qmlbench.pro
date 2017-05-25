TEMPLATE = subdirs
SUBDIRS = \
    src \
    tools

benchmarks.path = /root
benchmarks.files = benchmarks
INSTALLS += benchmarks

shared.path = /root
shared.files = shared
INSTALLS += shared
