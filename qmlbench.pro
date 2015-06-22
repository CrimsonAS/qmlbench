TEMPLATE = app
QT += quick

lessThan(QT_MAJOR_VERSION, 5): error("This benchmark tool is for QtQuick 2 only, and requires Qt 5.")

SOURCES += main.cpp

equals(QT_MAJOR_VERSION, 5): lessThan(QT_MINOR_VERSION, 2) {
    SOURCES += \
        compat/qcommandlineoption.cpp \
        compat/qcommandlineparser.cpp

    HEADERS += \
        compat/qcommandlineoption.h \
        compat/qcommandlineparser.h

    INCLUDEPATH += compat
}

CONFIG += console
CONFIG -= app_bundle

RESOURCES += qmlbench.qrc
