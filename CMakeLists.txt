cmake_minimum_required(VERSION 3.7)
project(demo)

set(CMAKE_SUPRESS_REGENERATION true)

set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

include_directories(${INCLUDE_DIRS})
add_executable(demo ${SOURCE_FILES})
target_link_libraries(demo)
