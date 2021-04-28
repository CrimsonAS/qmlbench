# Generated from src.pro.

cmake_minimum_required(VERSION 3.15.0)

qt_internal_add_executable(qmlbench
    OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}" # special case
    SOURCES
        benchmark.h
        benchmarkrunner.cpp benchmarkrunner.h
        main.cpp
        options.h
        qmlbench.cpp qmlbench.h
        resultrecorder.cpp resultrecorder.h
        testmodel.cpp testmodel.h
    PUBLIC_LIBRARIES
        Qt::Gui
        Qt::Quick
        Qt::Quick3D
)

# Resources:
set(qmlbench_resource_files
    "3rdparty/v8-bench.js"
    "Benchmark.qml"
    "CreationBenchmark.qml"
    "Shell_SustainedFpsWithCount.qml"
    "Shell_SustainedFpsWithStaticCount.qml"
    "Shell_TotalFramesWithStaticCount.qml"
    "V8Benchmark.qml"
)

qt_internal_add_resource(qmlbench "qmlbench"
    PREFIX
        "/"
    FILES
        ${qmlbench_resource_files}
)
