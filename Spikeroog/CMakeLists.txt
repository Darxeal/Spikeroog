cmake_minimum_required(VERSION 3.10)
project(cppbot LANGUAGES CXX)

# look in for the RLUtilities directory next door
add_subdirectory(../RLUtilities ${CMAKE_BINARY_DIR}/rlu)

# let CMake know which files to compile for your bot library
set(source_files
  ${PROJECT_SOURCE_DIR}/src/agent.cc
  ${PROJECT_SOURCE_DIR}/src/mechanics/travel.cc
  ${PROJECT_SOURCE_DIR}/src/mechanics/walldash.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/pickup_ball.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/challenge.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/kickoff.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/defense.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/spike_shot.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/spike_ceiling_shot.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/walldash_spike_carry.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/collect_boost.cc
  ${PROJECT_SOURCE_DIR}/src/maneuvers/recovery.cc
  ${PROJECT_SOURCE_DIR}/src/utils/vector_math.cc
)

# build the library
add_library(botlib STATIC ${source_files})
target_include_directories(botlib PUBLIC inc)
target_compile_features(botlib PUBLIC cxx_std_17)        
target_link_libraries(botlib PUBLIC rlutilities_cpp)
 
# build the executable
add_executable(my_bot main.cpp)
target_link_libraries(my_bot  PUBLIC rlutilities_cpp botlib)