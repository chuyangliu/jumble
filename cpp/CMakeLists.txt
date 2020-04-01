cmake_minimum_required(VERSION 3.10)

project(jumblecpp CXX)

set(TESTS
  test_shared_ptr
  test_unique_ptr
  test_doubly_linked_list
  test_binary_heap
  test_hash_table
  test_avl_tree
  test_graph
  test_sort
  test_npuzzle
  test_dijkstra
  test_md5
  test_calculator
)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src)

add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty)
include_directories(${PROJECT_SOURCE_DIR}/thirdparty)

include(CTest)

foreach (test ${TESTS})
  add_executable(${test} ${PROJECT_SOURCE_DIR}/tests/${test}.cpp)
  target_link_libraries(${test} jumblecpp catch2)
  add_test(${test} ${PROJECT_BINARY_DIR}/${test})
endforeach ()
