include_directories(${PROJECT_SOURCE_DIR})

add_executable(entangled_hydron main.cc hydron.cc database.cc)
TARGET_LINK_LIBRARIES(entangled_hydron common3d)
