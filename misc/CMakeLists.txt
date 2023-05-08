cmake_minimum_required(VERSION 2.6.4)

project(TastyLib CXX)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)  # Default build type
endif ()

option(TASTYLIB_BUILD_TEST "Build tastylib tests." OFF)

if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -std=c++11 ${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O2")
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "MSVC")
    # No config
endif ()
message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS_DEBUG: ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "CMAKE_CXX_FLAGS_RELEASE: ${CMAKE_CXX_FLAGS_RELEASE}")

# Headers
file(GLOB_RECURSE TASTYLIB_HEADERS "${PROJECT_SOURCE_DIR}/include/*.h")
include_directories(${PROJECT_SOURCE_DIR}/include)

# Sources
file(GLOB_RECURSE TASTYLIB_SRCS "${PROJECT_SOURCE_DIR}/src/tastylib/*.cpp")

# Tests
if (TASTYLIB_BUILD_TEST)
    include(CTest)
    add_subdirectory(${PROJECT_SOURCE_DIR}/test)
endif ()

# Benchmark executables
set(EXEC_LIST
    benchmark_DoublyLinkedList
    benchmark_BinaryHeap
    benchmark_AVLTree
    benchmark_MD5
    benchmark_NPuzzle
    benchmark_Sort)

foreach (exec ${EXEC_LIST})
    add_executable(${exec} ${PROJECT_SOURCE_DIR}/src/benchmark/${exec}.cpp
                   ${TASTYLIB_SRCS} ${TASTYLIB_HEADERS})
endforeach ()

# Sample executable
add_executable(sample ${PROJECT_SOURCE_DIR}/src/sample.cpp
                ${TASTYLIB_SRCS} ${TASTYLIB_HEADERS})
