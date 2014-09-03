cmake_minimum_required(VERSION 2.8)
project(entangled_hydron)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x -g ${CMAKE_CXX_FLAGS}")

add_subdirectory(common3d)
include_directories(common3d/include)

FIND_PACKAGE(Boost REQUIRED)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

add_subdirectory(src)
