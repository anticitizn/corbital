
cmake_minimum_required(VERSION 3.13)
project(corbital)

file(GLOB SOURCES src/*.cpp)
add_executable(corbital ${SOURCES} main.cpp)
add_subdirectory("external/omar")

target_link_libraries(corbital ${CMAKE_DL_LIBS} omar)
target_include_directories(corbital PUBLIC "${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
