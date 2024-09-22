cmake_minimum_required(VERSION 3.16)

project(srm-example ASM CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(".." crl)

add_resource_library(assets "${CMAKE_SOURCE_DIR}/assets/")
add_executable(example "main.cpp")
target_link_libraries(example PRIVATE assets crl_hot_reload)
