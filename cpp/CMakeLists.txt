cmake_minimum_required(VERSION 3.10)

project(jumble CXX)

set(TESTS
  test_doubly_linked_list
  test_binary_heap
  test_hash_table
  test_avl_tree
  test_graph
  test_sort
)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/thirdparty
)

include(CTest)
add_library(catch2 ${PROJECT_SOURCE_DIR}/thirdparty/catch2/catch_amalgamated.cpp)

foreach (test ${TESTS})
  add_executable(${test} ${PROJECT_SOURCE_DIR}/test/${test}.cpp)
  target_link_libraries(${test} catch2)
  add_test(${test} ${PROJECT_BINARY_DIR}/${test})
endforeach ()
