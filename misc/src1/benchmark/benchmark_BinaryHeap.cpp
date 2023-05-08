#include "tastylib/util/io.h"
#include "tastylib/util/timer.h"
#include "tastylib/util/random.h"
#include "tastylib/BinaryHeap.h"
#include <algorithm>
#include <queue>

using tastylib::printLn;
using tastylib::timing;
using tastylib::Random;
using tastylib::BinaryHeap;


int main() {
    printLn("Benchmark of BinaryHeap running...\n");
    {
        const int SIZE = 40000000;

        std::vector<int> vals;
        Random *random = Random::getInstance();

        std::priority_queue<int> stdHeap;
        BinaryHeap<int, std::less_equal<int>> libHeap;  // Max-root heap

        // Generate elements
        printLn("Generating ", SIZE, " elements to push...");
        for (int i = 0; i < SIZE; ++i) {
            vals.push_back(random->nextInt(0, SIZE));
        }
        printLn("Finished.\n");

        {   // Benchmark push()
            printLn("Benchmarking push()...");
            printLn("Pushing ", SIZE, " elements to the heap...");
            auto stdTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    stdHeap.push(vals[i]);
                }
            });
            printLn("std finished.");
            auto libTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    libHeap.push(vals[i]);
                }
            });
            printLn("lib finished.");
            printLn("Avg time of std VS TastyLib: ", stdTime / SIZE, " ms / ", libTime / SIZE, " ms");
            printLn("Benchmark of push() finished.\n");
        }

        {   // Benchmark pop()     
            printLn("Benchmarking pop()...");
            printLn("Poping ", SIZE, " elements from the heap...");
            auto stdTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    stdHeap.pop();
                }
            });
            printLn("std finished.");
            auto libTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    libHeap.pop();
                }
            });
            printLn("lib finished.");
            printLn("Avg time of std VS TastyLib: ", stdTime / SIZE, " ms / ", libTime / SIZE, " ms");
            printLn("Benchmark of pop() finished.\n");
        }

        {   // Benchmark makeHeap()     
            printLn("Benchmarking makeHeap()...");
            printLn("Building a ", SIZE, " elements heap...");
            auto stdTime = timing([&]() {
                std::vector<int> test = vals;
                std::make_heap(test.begin(), test.end());
            });
            printLn("std finished.");
            auto libTime = timing([&]() {
                BinaryHeap<int, std::less_equal<int>> libHeap(vals);
            });
            printLn("lib finished.");
            printLn("Time of std VS TastyLib: ", stdTime, " ms / ", libTime, " ms");
            printLn("Benchmark of makeHeap() finished.\n");
        }
    }
    printLn("Benchmark of BinaryHeap finished.");
    checkMemoryLeaks();
    return 0;
}
