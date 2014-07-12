include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(entangled_hydron main.cc hydron.cc colony.cc database.cc)
TARGET_LINK_LIBRARIES(entangled_hydron common3d)
