TARGET = qmlbench
TEMPLATE = app
QT += quick

lessThan(QT_MAJOR_VERSION, 5): error("This benchmark tool is for QtQuick 2 only, and requires Qt 5.")

SOURCES += \
    main.cpp \
    resultrecorder.cpp \
    benchmarkrunner.cpp \
    testmodel.cpp \
    qmlbench.cpp

HEADERS += \
    resultrecorder.h \
    benchmarkrunner.h \
    benchmark.h \
    options.h \
    testmodel.h \
    qmlbench.h

CONFIG += console
CONFIG -= app_bundle

RESOURCES += qmlbench.qrc

target.path = /root
INSTALLS += target
