cmake_minimum_required(VERSION 3.16)

option(BUILD_TESTING "Build the test suite" ON)

if(NOT DEFINED CUDAToolkit_ROOT AND WIN32)
    # Check common installation path on Windows
    set(CUDA_12_8_PATH "C:/Program Files/NVIDIA GPU Computing Toolkit/CUDA/v12.8")
    if(EXISTS "${CUDA_12_8_PATH}")
        set(CUDAToolkit_ROOT "${CUDA_12_8_PATH}" CACHE PATH "Path to CUDA 12.8 Toolkit")
        message(STATUS "Auto-detected CUDA 12.8 at: ${CUDAToolkit_ROOT}")
    endif()
endif()

if(BUILD_TESTING)
    set(VCPKG_MANIFEST_FEATURES "tests" CACHE STRING "" FORCE)
else()
    set(VCPKG_MANIFEST_FEATURES "" CACHE STRING "" FORCE)
endif()

#VCPGK

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_USE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)
set(VCPKG_FORCE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)

project(CTLab VERSION 0.1 LANGUAGES CXX CUDA)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(soil CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(freetype CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(CUDAToolkit 12.8 REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS 
    Core Qml
)

add_subdirectory(Model)
add_subdirectory(CTLab)
add_subdirectory(SaveVolumeToFile)
add_subdirectory(QuadControl)

#if(BUILD_TESTING)
#    enable_testing()
#    add_subdirectory(tests)
#endif()


