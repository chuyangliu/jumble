#include "tastylib/util/io.h"
#include "tastylib/util/timer.h"
#include "tastylib/util/random.h"
#include "tastylib/AVLTree.h"
#include <set>
#include <vector>

using tastylib::printLn;
using tastylib::timing;
using tastylib::Random;
using tastylib::AVLTree;


int main() {
    printLn("Benchmark of AVLTree running...\n");
    {
        const int SIZE = 10000000;

        std::vector<int> vals;
        Random *random = Random::getInstance();

        std::multiset<int> stdTree;
        AVLTree<int> libTree;

        // Generate elements
        printLn("Generating ", SIZE, " elements to benchmark...");
        for (int i = 0; i < SIZE; ++i) {
            vals.push_back(random->nextInt(0, SIZE));
        }
        printLn("Finished.\n");

        {   // Benchmark insert()
            printLn("Benchmarking insert()...");
            printLn("Inserting ", SIZE, " elements into the tree...");
            auto stdTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    stdTree.insert(vals[i]);
                }
            });
            printLn("std finished.");
            auto libTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    libTree.insert(vals[i]);
                }
            });
            printLn("lib finished.");
            printLn("Avg time of std VS TastyLib: ", stdTime / SIZE, " ms / ", libTime / SIZE, " ms");
            printLn("Benchmark of insert() finished.\n");
        }

        {   // Benchmark find()/has()
            printLn("Benchmarking find()/has()...");
            printLn("Finding ", SIZE, " elements in the tree...");
            bool correct = true;
            auto stdTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    if (stdTree.find(vals[i]) == stdTree.end()) {
                        correct = false;
                        break;
                    }
                }
            });
            printLn("std finished.");
            auto libTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    if (!libTree.has(vals[i])) {
                        correct = false;
                        break;
                    }
                }
            });
            printLn("lib finished.");
            printLn("Correctness check: ", correct ? "pass" : "fail");
            printLn("Avg time of std VS TastyLib: ", stdTime / SIZE, " ms / ", libTime / SIZE, " ms");
            printLn("Benchmark of find()/has() finished.\n");
        }

        {   // Benchmark remove()
            printLn("Benchmarking remove()...");
            printLn("Removing ", SIZE, " elements from the tree...");
            auto stdTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    stdTree.erase(vals[i]);
                }
            });
            printLn("std finished.");
            auto libTime = timing([&]() {
                for (int i = 0; i < SIZE; ++i) {
                    libTree.remove(vals[i]);
                }
            });
            printLn("lib finished.");
            printLn("Correctness check: ", (stdTree.empty() && libTree.isEmpty()) ? "pass" : "fail");
            printLn("Avg time of std VS TastyLib: ", stdTime / SIZE, " ms / ", libTime / SIZE, " ms");
            printLn("Benchmark of remove() finished.\n");
        }
    }
    printLn("Benchmark of AVLTree finished.");
    checkMemoryLeaks();
    return 0;
}
