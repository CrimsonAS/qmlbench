# Generated from qmlbench.pro.
# special case skip regeneration

cmake_minimum_required(VERSION 3.15.0)

include(.cmake.conf)
project(QMLBench
    VERSION "${QT_REPO_MODULE_VERSION}"
    DESCRIPTION "QMLBench" # special case
    HOMEPAGE_URL "https://qt.io/"
    LANGUAGES CXX C
)

# special case begin
# Make sure we only use latest private CMake API, aka no compatibility wrappers.
set(QT_NO_INTERNAL_COMPATIBILITY_FUNCTIONS TRUE)
# special case end

find_package(Qt6 ${PROJECT_VERSION} CONFIG REQUIRED COMPONENTS BuildInternals Core Gui Qml Quick Test)

qt_build_repo_begin()

add_subdirectory(src)
add_subdirectory(tools)

qt_build_repo_end()

