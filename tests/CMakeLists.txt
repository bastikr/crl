cmake_minimum_required(VERSION 3.24)
project(crl-tests)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CLANG_TIDY_COMMAND "clang-tidy")

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG v1.9.0
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest googlebenchmark)

enable_testing()

add_subdirectory(.. crl)

set_target_properties(crl PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
set_target_properties(crl_hot_reload PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

add_executable(test_utils_strv
    "src/test_utils_strv.cpp"
)

set_target_properties(test_utils_strv PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

target_link_libraries(test_utils_strv
    PRIVATE
        GTest::gtest_main
        crl
)

add_executable(crl_benchmark
  "src/bench.cpp"
)

add_resource_library(assets "${CMAKE_SOURCE_DIR}/../tools/build/testsets_num10_depth2/" GPERF)

target_link_libraries(crl_benchmark
    PRIVATE
        benchmark::benchmark
        assets
)
set_target_properties(crl_benchmark PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

include(GoogleTest)
gtest_discover_tests(test_utils_strv)
