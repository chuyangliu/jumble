cmake_minimum_required(VERSION 3.10)

project(libmisccpp LANGUAGES CXX)

set(TESTS
  doubly_linked_list_test
)

include(CTest)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/thirdparty
)

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(Catch2 ${PROJECT_SOURCE_DIR}/thirdparty/catch2/catch_amalgamated.cpp)

foreach (test ${TESTS})
  add_executable(${test} ${PROJECT_SOURCE_DIR}/test/${test}.cpp ${SOURCES})
  target_link_libraries(${test} Catch2)
  add_test(${test} ${PROJECT_BINARY_DIR}/${test})
endforeach ()
