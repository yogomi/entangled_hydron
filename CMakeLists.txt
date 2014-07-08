cmake_minimum_required(VERSION 2.8)
project(entangled_synapse)

if(CMAKE_COMPILER_IS_GNUCXX)
  list(APPEND CMAKE_CXX_FLAGS "-std=gnu++0x ${CMAKE_CXX_FLAGS}")
endif()

add_subdirectory(src)
