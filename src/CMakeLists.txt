include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(entangled_hydron
                  main.cc
                  hydron.cc
                  colony.cc
                  learning_theory/learning_theory.cc
                  learning_theory/keep_current.cc
                  learning_theory/feed_learning.cc
                  database.cc
                  random_number_generator.cc)
TARGET_LINK_LIBRARIES(entangled_hydron common3d)
