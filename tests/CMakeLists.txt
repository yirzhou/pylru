include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(target lru.t)

add_executable(${target} ${SRCS})

target_link_libraries(${target} PRIVATE
    lru
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${target})
