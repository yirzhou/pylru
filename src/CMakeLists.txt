set(target pylru)

target_link_libraries(${target} MODULE lru)
pybind11_add_module(${target} src/pylru.cpp)
target_include_directories(${target} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
