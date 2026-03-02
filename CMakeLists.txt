cmake_minimum_required(VERSION 3.20)
project(audio_fingerprint VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build type
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# Compiler flags
add_compile_options(-Wall -Wextra -Wpedantic)

# FFTW3
find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW3 REQUIRED fftw3f)

# Google Test
find_package(GTest REQUIRED)

# Google Benchmark
find_package(benchmark REQUIRED)

# Include dirs
include_directories(${CMAKE_SOURCE_DIR})

# Subdirectories
add_subdirectory(core/audio)
add_subdirectory(tests)
add_subdirectory(bench)
add_subdirectory(tools)