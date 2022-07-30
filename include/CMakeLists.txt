set(target lru)

include_directories(/Users/zhouyiren/opt/miniconda3/envs/native/include)
find_package(Python 3.10 COMPONENTS Interpreter Development REQUIRED)
find_package(pybind11 CONFIG REQUIRED)

add_library(${target} STATIC
    node.cpp
    linked_list.cpp
    lru.cpp
)

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
