cmake_minimum_required(VERSION 3.20)
project(my_qml_module VERSION 1.0)

# -----------------------------
# 1. Qt and PySide6
# -----------------------------
# Find PySide6 and Qt components
find_package(PySide6 6.5 REQUIRED)
find_package(Qt6 COMPONENTS Qml Quick OpenGL REQUIRED)


# Qt policies (must come after find_package)
qt_policy(SET QTP0001 NEW)
qt_standard_project_setup() 

# -----------------------------
# 2. Shiboken6 / Python bindings
# -----------------------------
# If CMake cannot find Shiboken6 automatically, point to the package manually
# set(Shiboken6Tools_DIR "C:/Files/PySideQmlTest/.venv/Lib/site-packages/shiboken6/lib/cmake/Shiboken6Tools")

# Find Shiboken6 and tools
find_package(Shiboken6Tools REQUIRED)
find_package(Shiboken6 REQUIRED)

# -----------------------------
# 3. C++ library (your QML item)
# -----------------------------
add_library(myqmlcpp SHARED volume_view_control_dummy.cpp)

target_include_directories(myqmlcpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(myqmlcpp PUBLIC Qt6::Qml Qt6::Quick Qt6::OpenGL)

# -----------------------------
# 4. Qt QML module
# -----------------------------
qt_add_qml_module(my_qml_module
    URI com.example.myqmlmodule
    VERSION 1.0
    SOURCES volumeViewControl.h
    RESOURCE_PREFIX /qt/qml/com/example/myqmlmodule
    OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/qml/com/example/myqmlmodule
)

# -----------------------------
# 5. Generate Python bindings with Shiboken6
# -----------------------------
message(STATUS "Shiboken6Tools_DIR=${Shiboken6Tools_DIR}")
set(Shiboken6Tools_DIR "C:/Files/PySideQmlTest/.venv/lib/site-packages/shiboken6_generator/lib/cmake/Shiboken6Tools")
find_package(Shiboken6Tools REQUIRED)
find_package(Shiboken6 REQUIRED)

message(STATUS "Shiboken6Tools_DIR=${Shiboken6Tools_DIR}")

shiboken6_generate_bindings(
    TARGET myqmlcpp
    SOURCES myqmlcpp.h
    TYPESYSTEM typesystem.xml
    OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/bindings"
    MODULE_NAME my_qml_module_bindings
)

# Setup the Python module