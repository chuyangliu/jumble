# Jumble

C/C++ implementations of data structures, algorithms, and anything else.

## C++

| Name | Sources | Notes |
|:----:|:-------:|-------|
|DoublyLinkedList|[tests](./cpp/test/test_doubly_linked_list.cpp) [.hpp](./cpp/src/jumble/doubly_linked_list.hpp)|Doubly linked list and merge sort.|
|BinaryHeap|[tests](./cpp/test/test_binary_heap.cpp) [.hpp](./cpp/src/jumble/binary_heap.hpp)|Heap implemented with complete binary tree.|
|HashTable|[tests](./cpp/test/test_hash_table.cpp) [.hpp](./cpp/src/jumble/hash_table.hpp)|Hash table with conflicting entries appended to linked list.|
|AVLTree|[tests](./cpp/test/test_avl_tree.cpp) [.hpp](./cpp/src/jumble/avl_tree.hpp)|Self-balancing binary search tree.|
|Graph|[tests](./cpp/test/test_graph.cpp) [.hpp](./cpp/src/jumble/graph.hpp)|Graph implemented with adjacency list/matrix.|
|Sort|[tests](./cpp/test/test_sort.cpp) [.hpp](./cpp/src/jumble/sort.hpp)|Sorting algorithms: insertion sort, selection sort, heap sort, quick sort, and quick select.|
|Dijkstra|[tests](./cpp/test/test_dijkstra.cpp) [.hpp](./cpp/src/jumble/dijkstra.hpp)|Algorithm to find shortest path between nodes in a graph.|
|MD5|[tests](./cpp/test/test_md5.cpp) [.hpp](./cpp/src/jumble/md5.hpp)|MD5 hash function.|
|Calculator|[tests](./cpp/test/test_calculator.cpp) [.hpp](./cpp/src/jumble/calculator.hpp) [.cpp](./cpp/src/jumble/calculator.cpp)|Infix arithmetic expression calculator based on [recursive descent parser](https://en.wikipedia.org/wiki/Recursive_descent_parser).|
|NPuzzle|[tests](./cpp/test/test_npuzzle.cpp) [.hpp](./cpp/src/jumble/npuzzle.hpp)|A* search to solve [N-Puzzle](https://en.wikipedia.org/wiki/15_puzzle) problem.|

## Installation

Go to target directory ([./cpp](./cpp) for C++, [./c](./c) for C) and build with [CMake](https://cmake.org/):

```
$ mkdir build
$ cd build
$ cmake ..
$ cmake --build .
```

C++ implementations come with unit tests which can be run with [CTest](https://cmake.org/cmake/help/latest/module/CTest.html):

```
$ ctest
```

C implementations don't have unit tests. Please run each example binary individually:

```
$ ./example_greeting
```

## License

See the [LICENSE](./LICENSE) file for license rights and limitations.
