cmake_minimum_required(VERSION 3.2)
project(entangled_hydron)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -g -DDEBUG ${CMAKE_CXX_FLAGS}")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" "/usr/local/share/cmake/Modules")

add_subdirectory(common3d)
include_directories(common3d/include)

# For Eigen
find_package(Eigen3 REQUIRED)
message("${EIGEN3_INCLUDE_DIR}")
include_directories(${EIGEN3_INCLUDE_DIR})

FIND_PACKAGE(Boost REQUIRED)
message("${Boost_INCLUDE_DIR}")
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
