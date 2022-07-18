cmake_minimum_required(VERSION 3.4...3.18)

set(CMAKE_CXX_STANDARD 17)
set(CLANG_CXX_LANGUAGE_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Add -O0 to remove optimizations when using gcc
IF(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

project(pylru LANGUAGES CXX)

enable_testing()

include_directories(/Users/zhouyiren/opt/miniconda3/envs/native/include)
find_package(pybind11 CONFIG REQUIRED)

add_subdirectory("src")

add_subdirectory("tests")

# pybind11_add_module(pylru src/lru.cpp)
