cmake_minimum_required(VERSION 3.10)

project(jumblecpp CXX)

set(TESTS
  test_doubly_linked_list
  test_binary_heap
)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

# file(GLOB_RECURSE LIBMISC_SRCS ${PROJECT_SOURCE_DIR}/src/*.cpp)
# add_library(misccpp ${LIBMISC_SRCS})

include(CTest)
add_library(catch2 ${PROJECT_SOURCE_DIR}/src/catch2/catch_amalgamated.cpp)

foreach (test ${TESTS})
  add_executable(${test} ${PROJECT_SOURCE_DIR}/test/${test}.cpp)
  target_link_libraries(${test} catch2)
  add_test(${test} ${PROJECT_BINARY_DIR}/${test})
endforeach ()
