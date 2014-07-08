include_directories(${CMAKE_SOURCE_DIR})

find_package(MYSQLPP REQUIRED)
include_directories(${MYSQLPP_INCLUDE_DIRS})

add_executable(entangled_synapse main.cc synapse.cc database.cc)
target_link_libraries(entangled_synapse ${MYSQLPP_LIBRARIES})
