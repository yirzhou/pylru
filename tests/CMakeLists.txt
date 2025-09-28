include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

FetchContent_MakeAvailable(googletest)

# Unit tests
set(test_target lru.t)

add_executable(${test_target} lru.t.cpp)

target_link_libraries(${test_target} PRIVATE
    lru
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${test_target})

# Performance tests
set(perf_target lru.perf)

add_executable(${perf_target} performance_test.cpp)

target_link_libraries(${perf_target} PRIVATE lru)
