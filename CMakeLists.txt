cmake_minimum_required(VERSION 2.8)
project(entangled_hydron)

list(APPEND CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")

add_subdirectory(common3d)
add_subdirectory(src)
