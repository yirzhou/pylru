find_package(Python 3.10 COMPONENTS Interpreter Development.Module REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

set(target pylru)

pybind11_add_module(${target} pylru.cpp)
target_link_libraries(${target} PRIVATE lru)
target_include_directories(${target} PRIVATE "${CMAKE_SOURCE_DIR}/include")
