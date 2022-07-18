cmake_minimum_required(VERSION 3.4...3.18)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(pylru LANGUAGES CXX)

enable_testing()

include_directories(/Users/zhouyiren/opt/miniconda3/envs/native/include)
find_package(pybind11 CONFIG REQUIRED)

add_subdirectory("src")

add_subdirectory("tests")

# pybind11_add_module(pylru src/lru.cpp)
