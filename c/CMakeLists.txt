cmake_minimum_required(VERSION 3.10)

project(jumblec C)

set(EXAMPLES
  example_greeting
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src)

foreach (example ${EXAMPLES})
  add_executable(${example} ${PROJECT_SOURCE_DIR}/examples/${example}.c)
  target_link_libraries(${example} jumblec)
endforeach ()
