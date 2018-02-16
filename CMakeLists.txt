cmake_minimum_required(VERSION 3.7)
project(binfs)

set(CMAKE_SUPRESS_REGENERATION true)

set(CMAKE_CXX_FLAGS "-std=c++11 -O3")
set(INCLUDE_DIRS ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

if (WIN32)
  set(INCLUDE_DIRS "${INCLUDE_DIRS}"
    "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.1A/Include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.16299.0/ucrt")
endif()

include_directories(${INCLUDE_DIRS})
add_executable(binfs ${SOURCE_FILES})
target_link_libraries(binfs)
