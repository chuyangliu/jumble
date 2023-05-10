include_directories(${PROJECT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/jumble/*.c)
add_library(jumblec ${SOURCES})
