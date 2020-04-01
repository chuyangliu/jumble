# Jumble

C/C++ implementations of data structures, algorithms, and common designs.

| Name | Sources | Notes |
|:----:|:-------:|-------|
|SharedPtr|[test](./cpp/tests/test_shared_ptr.cpp) [.hpp](./cpp/src/jumble/shared_ptr.hpp)|My own version of [std::shared_ptr](https://en.cppreference.com/w/cpp/memory/shared_ptr).|
|UniquePtr|[test](./cpp/tests/test_unique_ptr.cpp) [.hpp](./cpp/src/jumble/unique_ptr.hpp)|My own version of [std::unique_ptr](https://en.cppreference.com/w/cpp/memory/unique_ptr).|
|Linked List|[test](./cpp/tests/test_doubly_linked_list.cpp) [.hpp](./cpp/src/jumble/doubly_linked_list.hpp)|Doubly linked list and merge sort.|
|Binary Heap|[test](./cpp/tests/test_binary_heap.cpp) [.hpp](./cpp/src/jumble/binary_heap.hpp)|Heap implemented with complete binary tree.|
|Hash Table|[test](./cpp/tests/test_hash_table.cpp) [.hpp](./cpp/src/jumble/hash_table.hpp)|Hash table with conflicting entries appended to linked list.|
|AVL Tree|[test](./cpp/tests/test_avl_tree.cpp) [.hpp](./cpp/src/jumble/avl_tree.hpp)|Self-balancing binary search tree.|
|Graph|[test](./cpp/tests/test_graph.cpp) [.hpp](./cpp/src/jumble/graph.hpp)|Graph implemented with adjacency list and matrix.|
|Sorting|[test](./cpp/tests/test_sort.cpp) [.hpp](./cpp/src/jumble/sort.hpp)|Common sorting algorithms.|
|N-Puzzle|[test](./cpp/tests/test_npuzzle.cpp) [.hpp](./cpp/src/jumble/npuzzle.hpp)|A* search to solve the [N-Puzzle](https://en.wikipedia.org/wiki/15_puzzle) problem.|
|Dijkstra|[test](./cpp/tests/test_dijkstra.cpp) [.hpp](./cpp/src/jumble/dijkstra.hpp)|Algorithm to find shortest path between nodes in a graph.|
|MD5|[test](./cpp/tests/test_md5.cpp) [.hpp](./cpp/src/jumble/md5.hpp)|MD5 hash function.|
|Calculator|[test](./cpp/tests/test_calculator.cpp) [.hpp](./cpp/src/jumble/calculator.hpp) [.cpp](./cpp/src/jumble/calculator.cpp)|Infix arithmetic expression calculator based on [recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser).|
|Web Server|[example](./c/examples/example_websvr.c) [.h](./c/src/jumble/websvr.h) [.c](./c/src/jumble/websvr.c)|Single-threaded web server.|
|Greeting|[example](./c/examples/example_greeting.c) [.h](./c/src/jumble/greeting.h) [.c](./c/src/jumble/greeting.c)|Funny [ASCII art](https://en.wikipedia.org/wiki/ASCII_art) for a greeting.|

## Installation

Build with [CMake](https://cmake.org/):

```
mkdir build
cd build
cmake ..
cmake --build .
```

C++ builds will be located at `build/cpp`. C++ implementations come with unit tests which can be run with [CTest](https://cmake.org/cmake/help/latest/module/CTest.html):

```
ctest --test-dir cpp
```

C builds will be located at `build/c`. C implementations don't have unit tests. Please run each binary individually:

```
./c/example_websvr
```

## License

See the [LICENSE](./LICENSE) file for license rights and limitations.
