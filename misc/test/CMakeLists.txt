cmake_minimum_required(VERSION 2.6.4)

enable_testing()

if (WIN32 AND (NOT CYGWIN) AND (NOT MINGW))
    # Prevent overriding the parent project's
    # compiler/linker settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    set(gtest_disable_pthreads ON)
endif ()

# Googletest module
add_subdirectory(${PROJECT_SOURCE_DIR}/thirdparty/gtest ${CMAKE_BINARY_DIR}/googletest)
include_directories(${gtest_SOURCE_DIR}/include)

# Executables
set(TEST_EXEC_LIST
    test_DoublyLinkedList
    test_BinaryHeap
    test_HashTable
    test_AVLTree
    test_Graph
    test_MD5
    test_NPuzzle
    test_Sort
    test_Dijkstra
    test_LCS
    test_TextQuery
    test_SharedPtr
    test_UniquePtr
    test_Calculator)

foreach (exec ${TEST_EXEC_LIST})
    add_executable(${exec} ${exec}.cpp ${TASTYLIB_SRCS} ${TASTYLIB_HEADERS})
    target_link_libraries(${exec} gtest gtest_main)
    add_test(${exec} ${EXECUTABLE_OUTPUT_PATH}/${exec})
endforeach ()
