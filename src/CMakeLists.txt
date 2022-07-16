set(target lru)

add_library(${target} STATIC
    node.cpp
    linked_list.cpp
    lru.cpp
)

target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
