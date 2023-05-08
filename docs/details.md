# Details

- [DoublyLinkedList](#doublylinkedlist)
- [BinaryHeap](#binaryheap)
- [HashTable](#hashtable)
- [AVLTree](#avltree)
- [Graph](#graph)
- [MD5](#md5)
- [Sort](#sort)
- [Dijkstra](#dijkstra)
- [LCS](#lcs)
- [NPuzzle](#npuzzle)
- [TextQuery](#textquery)
- [SharedPtr](#sharedptr)
- [UniquePtr](#uniqueptr)
- [Calculator](#calculator)

## DoublyLinkedList

### Usage

```c++
#include "tastylib/DoublyLinkedList.h"

using namespace tastylib;

int main() {
    DoublyLinkedList<int> list;

    auto isEmpty = list.isEmpty();  // isEmpty == true

    list.insertBack(1);   // List content: 1
    list.insertFront(2);  // List content: 2 1
    list.insert(1, 3);    // List content: 2 3 1
    list.insert(3, 4);    // List content: 2 3 1 4
    list.sort();          // List content: 1 2 3 4

    auto p1 = list.find(3);  // p1 == 2

    list.remove(p1);     // List content: 1 2 4
    list.removeFront();  // List content: 2 4
    list.removeBack();   // List content: 2

    auto p2 = list.find(3);  // p2 == -1

    auto size = list.getSize();  // size == 1

    return 0;
}
```

### Benchmark

#### Cost in theory

| Operation | Time |
|:---------:|:----:|
|insertFront()|O(1)|
|removeFront()|O(1)|
|insertBack()|O(1)|
|removeBack()|O(1)|
|insert()|O(n)|
|remove()|O(n)|
|find()|O(n)|
|sort() (merge sort)|O(nlogn)|

#### Cost in practice

Source: [benchmark_DoublyLinkedList.cpp](../src/benchmark/benchmark_DoublyLinkedList.cpp)

The program compares the time cost of `DoublyLinkedList` with `std::list`. When benchmarking `find()` and `sort()`, the size of the list is **100,000** and **5,000,000**, respectively. Here are the results under different environments:

##### Ubuntu 16.04 64-bit / g++ 5.4

| Operation | std::list | DoublyLinkedList |
|:---------:|:---------:|:----------------:|
|insertFront()|29 ns|26 ns|
|removeFront()|13 ns|12 ns|
|insertBack()|29 ns|27 ns|
|removeBack()|13 ns|12 ns|
|find()|157 µs|218 µs|
|sort()|3478 ms|3084 ms|

##### Windows 10 64-bit / Visual Studio 14 2015

| Operation | std::list | DoublyLinkedList |
|:---------:|:---------:|:----------------:|
|insertFront()|52 ns|51 ns|
|removeFront()|39 ns|44 ns|
|insertBack()|55 ns|48 ns|
|removeBack()|43 ns|47 ns|
|find()|214 µs|218 µs|
|sort()|3171 ms|2699 ms|

## BinaryHeap

### Usage

```c++
#include "tastylib/BinaryHeap.h"
#include <vector>

using namespace tastylib;

int main() {
    BinaryHeap<int> heap1;  // Create a min-root heap

    auto isEmpty = heap1.isEmpty();  // isEmpty == true

    heap1.push(50);
    heap1.push(20);
    heap1.push(30);

    auto size1 = heap1.getSize();  // size1 == 3

    auto val1 = heap1.top();  // val1 == 20
    heap1.pop();
    auto val2 = heap1.top();  // val2 == 30
    heap1.pop();
    auto val3 = heap1.top();  // val3 == 50
    heap1.pop();

    auto size2 = heap1.getSize();  // size2 == 0

    std::vector<int> vals;
    vals.push_back(50);
    vals.push_back(20);
    vals.push_back(30);

    BinaryHeap<int> heap2(vals);  // Build heap using a vector

    auto size3 = heap2.getSize();  // size3 == 3

    auto val4 = heap2.top();  // val4 == 20
    heap2.pop();
    auto val5 = heap2.top();  // val5 == 30
    heap2.pop();
    auto val6 = heap2.top();  // val6 == 50
    heap2.pop();

    auto size4 = heap2.getSize();  // size4 == 0

    return 0;
}
```

### Benchmark

#### Cost in theory

| Operation | Time |
|:---------:|:----:|
|push()|O(logn)|
|top()|O(1)|
|pop()|O(logn)|
|makeHeap()|O(n)|

#### Cost in practice

Source: [benchmark_BinaryHeap.cpp](../src/benchmark/benchmark_BinaryHeap.cpp)

The program compares the time cost of `BinaryHeap` with `std::priority_queue` and `std::make_heap`. It calculates the average time cost of each operation. Here are the results under different environments:

##### Ubuntu 16.04 64-bit / g++ 5.4

| Operation | std::priority_queue / std::make_heap | BinaryHeap |
|:---------:|:------------------------------------:|:----------:|
|push()|18 ns|17 ns|
|pop()|259 ns|271 ns|
|makeHeap()|455 ms|434 ms|

##### Windows 10 64-bit / Visual Studio 14 2015

| Operation | std::priority_queue / std::make_heap | BinaryHeap |
|:---------:|:------------------------------------:|:----------:|
|push()|22 ns|21 ns|
|pop()|592 ns|267 ns|
|makeHeap()|430 ms|515 ms|

## HashTable

### Usage

```c++
#include "tastylib/HashTable.h"
#include <string>

using namespace tastylib;

int main() {
    HashTable<std::string> table;

    auto isEmpty = table.isEmpty();  // isEmpty == true

    table.insert("Alice");
    table.insert("Darth");

    auto size1 = table.getSize();  // size1 == 2

    auto hasAlice = table.has("Alice");  // hasAlice == true
    auto hasDarth = table.has("Darth");  // hasDarth == true

    table.remove("Darth");

    hasAlice = table.has("Alice");  // hasAlice == true
    hasDarth = table.has("Darth");  // hasDarth == false

    auto size2 = table.getSize();  // size2 == 1

    return 0;
}
```

### Benchmark

#### Cost in theory

| Operation | Time |
|:---------:|:----:|
|insert()|O(1)|
|has()/find()|O(1)|
|remove()|O(1)|
|rehash()|O(n)|

#### Cost in practice

Note that there are many different ways to implement the hash table. The MSVC compiler implements the `std::unordered_set` as a **dynamic** hash table, which means that its bucket amount changes dynamically when performing `insert()` and `remove()/erase()` operations (i.e., using [extendible hashing](https://en.wikipedia.org/wiki/Extendible_hashing) or [linear hashing](https://en.wikipedia.org/wiki/Linear_hashing)). While in TastyLib, for simplicity, the hash table is **static** so its bucket amount is fixed after initialized. Since different implementations have different pros and cons, it's hard to give a convincing benchmark result.

## AVLTree

### Usage

```c++
#include "tastylib/AVLTree.h"
#include <string>

using namespace tastylib;

int main() {
    AVLTree<int> tree;

    auto isEmpty = tree.isEmpty();  // isEmpty == true

    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(3);

    std::string str1 = tree.preorder();   // str1 == "{2, 1, 3, 3}"
    std::string str2 = tree.inorder();    // str2 == "{1, 2, 3, 3}"
    std::string str3 = tree.postorder();  // str3 == "{1, 3, 3, 2}"

    auto size1 = tree.getSize();  // size1 == 4
    auto found1 = tree.has(3);    // found1 == true

    tree.remove(3);

    std::string str4 = tree.preorder();  // str4 == "{2, 1}"

    auto size2 = tree.getSize();  // size2 == 2
    auto found2 = tree.has(3);    // found2 == false

    return 0;
}
```

### Benchmark

#### Cost in theory

| Operation | Time |
|:---------:|:----:|
|find()|O(logn)|
|insert()|O(logn)|
|remove()|O(logn)|

#### Cost in practice

Source: [benchmark_AVLTree.cpp](../src/benchmark/benchmark_AVLTree.cpp)

The program compares the time cost of `AVLTree` with `std::multiset`. It calculates the average time cost of each operation. Note that the `std::multiset` is implemented as a [red-black tree](https://en.wikipedia.org/wiki/Red%E2%80%93black_tree), which is faster than the AVL tree when performing `insert()` and `remove()` operations but slower when performing `find()`. Here are the results under different environments:

##### Ubuntu 16.04 64-bit / g++ 5.4

| Operation | std::multiset | AVLTree |
|:---------:|:-------------:|:-------:|
|find()|1274 ns|1012 ns|
|insert()|1218 ns|1429 ns|
|remove()|1272 ns|1549 ns|

##### Windows 10 64-bit / Visual Studio 14 2015

| Operation | std::multiset | AVLTree |
|:---------:|:-------------:|:-------:|
|find()|1077 ns|816 ns|
|insert()|1098 ns|1213 ns|
|remove()|1405 ns|1485 ns|

## Graph

### Usage

```c++
#include "tastylib/Graph.h"
#include <string>

using namespace tastylib;

int main() {

    // Create a graph that has three vertices.
    // Each vertex stores a string object.
    Graph<std::string> graph(3);

    // Modify the string object in each vertex
    graph[0] = "I am vertex 0.";
    graph[1] = "I am vertex 1.";
    graph[2] = "I am vertex 2.";

    // Add edges
    graph.setWeight(0, 1, 10);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 30);

    // Get edge weights
    auto w0 = graph.getWeight(0, 1);  // w0 == 10
    auto w1 = graph.getWeight(0, 2);  // w1 == 20
    auto w2 = graph.getWeight(1, 2);  // w2 == 30

    // Get adjacent vertices
    auto n0 = graph.getNeighbors(0);  // n0 == [1, 2]
    auto n1 = graph.getNeighbors(1);  // n1 == [2]
    auto n2 = graph.getNeighbors(2);  // n2 == []

    return 0;
}
```

## MD5

### Usage

```c++
#include "tastylib/MD5.h"

using namespace tastylib;

int main() {

    // hashedMsg == "2dabbfd553b67530e4892eb9481121fa",
    // which is the MD5 value of the message "TastyLib"
    std::string hashedMsg = MD5<>::getInstance()->hash("TastyLib");

    return 0;
}
```

### Benchmark

Source: [benchmark_MD5.cpp](../src/benchmark/benchmark_MD5.cpp)

The program uses the MD5 algorithm to hash a fixed message of 200 MB for several times and calculates the average time cost. Here are the results:

| Environment | Average Time |
|-------------|:------------:|
|Ubuntu 16.04 64-bit / g++ 5.4|834 ms|
|Windows 10 64-bit / Visual Studio 14 2015|1140 ms|

## Sort

### Usage

```c++
#include "tastylib/Sort.h"

using namespace tastylib;

int main() {
    const unsigned n = 5;
    int arr[n] = {5, 4, 3, 2, 1};

    {   // Sort.
        // Aftering running each of the function below,
        // the array's content will be: [1, 2, 3, 4, 5]
        insertionSort(arr, n);
        selectionSort(arr, n);
        heapSort(arr, n);
        quickSort(arr, 0, n - 1);
    }

    {   // Find the kth smallest element.
        // After running the function below, the kth
        // smallest element will be stored at arr[k].
        int k = 1;  // Find the second smallest element
        quickSelect(arr, 0, n - 1, k);
    }

    return 0;
}
```

### Benchmark

#### Cost in theory

| Operation | Time | Stable |
|:---------:|:----:|:------:|
|insertionSort()|O(n^2)|Yes|
|selectionSort()|O(n^2)|No|
|heapSort()|O(nlogn)|No|
|mergeSort()|O(nlogn)|Yes|
|quickSort()|O(nlogn)|No|
|quickSelect()|O(n)|-|

#### Cost in practice

Source: [benchmark_Sort.cpp](../src/benchmark/benchmark_Sort.cpp)

The program calculates the average time cost to sort or find the kth element in an array of `100000` elements. Here are the results under different environments:

##### Ubuntu 16.04 64-bit / g++ 5.4

| Operation | Time |
|:---------:|:----:|
|std::sort()|6.77 ms|
|insertionSort()|1787.78 ms|
|selectionSort()|12297.26 ms|
|heapSort()|10.96 ms|
|quickSort()|7.15 ms|
|std::nth_element()|0.82 ms|
|quickSelect()|0.82 ms|

##### Windows 10 64-bit / Visual Studio 14 2015

| Operation | Time |
|:---------:|:----:|
|std::sort()|8.60 ms|
|insertionSort()|1197.20 ms|
|selectionSort()|4211.20 ms|
|heapSort()|11.10 ms|
|quickSort()|6.40 ms|
|std::nth_element()|0.80 ms|
|quickSelect()|0.90 ms|

## Dijkstra

### Usage

```c++
#include "tastylib/Dijkstra.h"
#include <string>

using namespace tastylib;

int main() {
    DijkGraph<string> graph(3);  // Create a graph that has three vertices

    graph[0].val = "Alice";  // Vertex 0 denotes Alice's home
    graph[1].val = "Darth";  // Vertex 1 denotes Darth's home
    graph[2].val = "Bob";    // Vertex 2 denotes Bob' home

    graph.setWeight(0, 1, 5);   // Distance from Alice's home to Darth's is 5
    graph.setWeight(0, 2, 20);  // Distance from Alice's home to Bob's is 20
    graph.setWeight(1, 2, 5);   // Distance from Darth's home to Bob's is 5

    // Run Dijkstra's algorithm to compute the
    // shortest path from Alice's home to others'.
    dijkstra(graph, 0);

    // Now I know the minimum distance from Alice's home to Bob's home, which is 10.
    auto distToBob = graph[2].dist;  // distToBob == 10

    // I also know the minimum path to Bob's home is "0->1->2", namely "Alice->Darth->Bob".
    auto prev1 = graph[2].prev;      // prev1 == 1
    auto prev2 = graph[prev1].prev;  // prev2 == 0

    return 0;
}
```

## LCS

### Usage

```c++
#include "tastylib/LCS.h"

using namespace tastylib;

int main() {

    // res1 == 4
    auto res1 = LCSubseq("0x2x3x4x5", "a12345a");

    // res2 == 1
    auto res2 = LCSubstr("0x2x3x4x5", "a12345a");

    return 0;
}
```

## NPuzzle

### Usage

```c++
#include "tastylib/NPuzzle.h"

using namespace tastylib;

int main() {

    // The beginning node and the ending node of a 3*3 puzzle problem.
    // Number '0' indicates the empty grid and number '1-8' denote other eight grids.
    PuzzleNode<> beg({0, 2, 3, 1, 4, 5, 6, 7, 8}, 3, 3);
    PuzzleNode<> end({1, 2, 3, 4, 0, 5, 6, 7, 8}, 3, 3);

    // Solve the problem
    NPuzzle<> puzzle(beg, end);
    puzzle.solve();

    // List 'path' stores the move directions from the beginning node
    // to the ending node. Its contents must be [DOWN, RIGHT].
    std::list<Direction> path = puzzle.getPath();

    return 0;
}
```

### Benchmark

Source: [benchmark_NPuzzle.cpp](../src/benchmark/benchmark_NPuzzle.cpp)

The program solves `3*3`, `4*4`, `5*5` and `6*6` puzzle problems respectively and generates the information of overheads. Here are the outputs of the benchmark under different environments:

##### Ubuntu 16.04 64-bit / g++ 5.4

```
Benchmark of NPuzzle running...

Benchmarking 3*3 puzzle...
Beg: {1, 4, 3, 8, 6, 5, 2, 7, 0}
End: {5, 8, 2, 0, 4, 3, 6, 7, 1}
Searching...
Searched nodes: 278
     Time cost: 3 ms
    Efficiency: 72.245322 node/ms
   Path length: 69
Solution check: pass
Benchmark of 3*3 puzzle finished.

Benchmarking 4*4 puzzle...
Beg: {5, 15, 2, 14, 4, 11, 12, 7, 6, 10, 3, 8, 0, 9, 1, 13}
End: {6, 15, 14, 7, 11, 13, 10, 3, 4, 8, 12, 1, 0, 5, 2, 9}
Searching...
Searched nodes: 842
     Time cost: 12 ms
    Efficiency: 68.433030 node/ms
   Path length: 100
Solution check: pass
Benchmark of 4*4 puzzle finished.

Benchmarking 5*5 puzzle...
Beg: {1, 22, 7, 2, 3, 0, 15, 17, 14, 4, 12, 23, 21, 9, 10, 8, 13, 16, 19, 24, 20, 11, 5, 18, 6}
End: {6, 5, 10, 4, 14, 12, 2, 3, 8, 18, 7, 15, 0, 22, 21, 13, 1, 11, 16, 9, 19, 20, 23, 24, 17}
Searching...
Searched nodes: 3869
     Time cost: 51 ms
    Efficiency: 75.225541 node/ms
   Path length: 213
Solution check: pass
Benchmark of 5*5 puzzle finished.

Benchmarking 6*6 puzzle...
Beg: {15, 12, 3, 0, 11, 8, 26, 29, 25, 4, 28, 1, 14, 13, 30, 5, 16, 17, 18, 7, 24, 32, 10, 21, 27, 20, 34, 31, 22, 9, 19, 2, 33, 6, 23, 35}
End: {8, 6, 9, 7, 11, 10, 22, 27, 4, 19, 1, 5, 18, 12, 25, 3, 14, 16, 30, 2, 35, 13, 15, 23, 32, 24, 31, 34, 33, 0, 21, 20, 26, 29, 28, 17}
Searching...
Searched nodes: 93068
     Time cost: 1478 ms
    Efficiency: 62.968877 node/ms
   Path length: 588
Solution check: pass
Benchmark of 6*6 puzzle finished.

Benchmark of NPuzzle finished.
```

## TextQuery

### Usage

```c++
#include "tastylib/TextQuery.h"
#include <string>
#include <sstream>
#include <iostream>

using namespace tastylib;

int main() {

    const std::string article =
        "Alice Emma has long flowing red hair\n"
        "Her Daddy says when the wind blows\n"
        "like a fiery bird in flight.\n"
        "through her hair, it looks almost alive,\n"
        "A beautiful fiery bird, he tells her,\n"
        "magical but untamed.\n"
        "\"Daddy, shush, there is no such thing,\"\n"
        "she tells him, at the same time wanting\n"
        "him to tell her more.\n"
        "Shyly, she asks, \"I mean, Daddy, is there?\"\n";

    std::istringstream iss(article);
    TextQuery tq(iss);

    /*
    Print:
    Daddy occurs: 3
    (line 2) Her Daddy says when the wind blows
    (line 7) "Daddy, shush, there is no such thing,"
    (line 10) Shyly, she asks, "I mean, Daddy, is there?"
    */
    std::cout << Query("Daddy").eval(tq);

    /*
    Print:
    (but | (there & ~(thing))) occurs: 2
    (line 6) magical but untamed.
    (line 10) Shyly, she asks, "I mean, Daddy, is there?"
    */
    std::cout << (Query("but") | (Query("there") & ~Query("thing"))).eval(tq);

    return 0;
}
```

### Design

![](./images/textquery_class_diagram.png)

<br>

![](./images/textquery_example.png)

## SharedPtr

### Usage

```c++
#include "tastylib/SharedPtr.h"
#include <string>
#include <utility>

using namespace tastylib;

int main() {

    SharedPtr<std::string> p1;
    auto cnt = p1.useCount();  // cnt == 0
    auto ptr = p1.get();       // ptr == nullptr

    p1 = SharedPtr<std::string>(new std::string("hello world"));
    auto cnt = p1.useCount();  // cnt == 1

    SharedPtr<std::string> p2 = p1;
    cnt = p1.useCount();  // cnt == 2
    cnt = p2.useCount();  // cnt == 2

    p1.reset();
    cnt = p1.useCount();  // cnt == 0
    cnt = p2.useCount();  // cnt == 1

    SharedPtr<std::string> p3 = std::move(p2);
    cnt = p1.useCount();  // cnt == 0
    cnt = p2.useCount();  // cnt == 0
    cnt = p3.useCount();  // cnt == 1

    p2 = p3;
    cnt = p1.useCount();  // cnt == 0
    cnt = p2.useCount();  // cnt == 2
    cnt = p3.useCount();  // cnt == 2

    p1 = std::move(p2);
    cnt = p1.useCount();  // cnt == 2
    cnt = p2.useCount();  // cnt == 0
    cnt = p3.useCount();  // cnt == 2

    p1.reset();
    cnt = p1.useCount();  // cnt == 0
    cnt = p2.useCount();  // cnt == 0
    cnt = p3.useCount();  // cnt == 1

    p3.reset();
    cnt = p1.useCount();  // cnt == 0
    cnt = p2.useCount();  // cnt == 0
    cnt = p3.useCount();  // cnt == 0

    return 0;
}
```

## UniquePtr

### Usage

```c++
#include "tastylib/UniquePtr.h"
#include <string>
#include <utility>

using namespace tastylib;

int main() {

    UniquePtr<std::string> p1(new std::string("hello world"));
    auto str = *p1.get();  // str == "hello world"

    UniquePtr<std::string> p2(std::move(p1));
    auto ptr = p1.get();   // ptr == nullptr
    str = *p2.get();       // str == "hello world"

    UniquePtr<std::string> p3;
    p3 = std::move(p2);
    ptr = p1.get();        // ptr == nullptr
    ptr = p2.get();        // ptr == nullptr
    str = *p3.get();       // ptr == "hello world"

    p3.reset();
    ptr = p1.get();        // ptr == nullptr
    ptr = p2.get();        // ptr == nullptr
    ptr = p3.get();        // ptr == nullptr

    return 0;
}
```

## Calculator

### Usage

```c++
#include "tastylib/Calculator.h"
#include <sstream>

using namespace tastylib;

int main() {

    std::istringstream iss("-3*20-(5+10/2)")
    auto res = Calculator(iss).calculate();  // res == -70

    return 0;
}
```

### Grammar

CFG (context-free grammar):

```
expr -> expr + A
      | expr - A
      | A

A -> A * B
   | A / B
   | B

B -> + B
   | - B
   | factor

factor -> ( expr )
        | num
```

SDT (syntax-directed translation scheme):

```
expr -> expr + A  { append('+'); }
      | expr - A  { append('-'); }
      | A

A -> A * B  { append('*'); }
   | A / B  { append('/'); }
   | B

B -> + B  { append('@'); }
   | - B  { append('#'); }
   | factor

factor -> ( expr )
        | num  { append(num); }
```

Eliminate left recursion:

```
expr -> A tmpA

tmpA -> + A  { append('+'); }  tmpA
      | - A  { append('-'); }  tmpA
      | ε

A -> B tmpB

tmpB -> * B  { append('*'); }  tmpB
      | / B  { append('/'); }  tmpB
      | ε

B -> + B  { append('@'); }
   | - B  { append('#'); }
   | factor

factor -> ( expr )
        | num  { append(num); }
```

First and Follow:

| Nonterminal | First | Follow |
|:-----------:|-------|--------|
|expr|+, -, (, num|$, )|
|A|+, -, (, num|$, ), +, -|
|tmpA|+, -, ε|$, )|
|B|+, -, (, num|$, ), +, -, *, /|
|tmpB|*, /, ε|$, ), +, -, |
|factor|(, num|$, ), +, -, *, /|

Predictive table (white-blanks denote error):

|   | + | - | * | / | ( | ) | num | $ |
|:-:|:-:|:-:|:-:|:-:|:-:|:-:|:---:|:-:|
|expr|expr -> A tmpA|expr -> A tmpA| | |expr -> A tmpA| |expr -> A tmpA| |
|A|A -> B tmpB|A -> B tmpB| | |A -> B tmpB| |A -> B tmpB| |
|tmpA|tmpA -> + A tmpA|tmpA -> - A tmpA| | | |tmpA -> ε| |tmpA -> ε|
|B|B -> + B|B -> - B| | |B -> factor| |B -> factor| |
|tmpB|tmpB -> ε|tmpB -> ε|tmpB -> * B tmpB|tmpB -> / B tmpB| |tmpB -> ε| |tmpB -> ε|
|factor| | | | |factor -> ( expr )| |factor -> num| |
