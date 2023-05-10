include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/jumble/*.cpp)
add_library(jumblecpp ${SOURCES})
