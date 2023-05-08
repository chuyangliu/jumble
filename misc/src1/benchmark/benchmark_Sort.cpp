#include "tastylib/util/io.h"
#include "tastylib/util/timer.h"
#include "tastylib/util/random.h"
#include "tastylib/Sort.h"
#include <algorithm>

using tastylib::printLn;
using tastylib::timing;
using tastylib::Random;
using tastylib::insertionSort;
using tastylib::selectionSort;
using tastylib::heapSort;
using tastylib::quickSort;
using tastylib::quickSelect;


int main() {
    printLn("Benchmark of Sort running...\n");
    {
        const int SIZE = 100000;
        const int CASES = 10;
        const int METHOD_NUM = 7;

        Random *random = Random::getInstance();

        // Total time cost of each sorting method
        std::vector<double> time(METHOD_NUM, 0);

        // Array of each sorting method
        std::vector<int*> method(METHOD_NUM, nullptr);
        for (int i = 0; i < METHOD_NUM; ++i) {
            method[i] = new int[SIZE];
        }

        // Running sorting cases
        printLn("Running ", CASES, " cases...\n");
        for (int i = 0; i < CASES; ++i) {
            printLn("Case #", i + 1, ":");
            printLn("Preparing ", SIZE, " elements to sort...");
            for (int j = 0; j < SIZE; ++j) {
                int e = random->nextInt(0, SIZE);
                for (int k = 0; k < METHOD_NUM; ++k) {
                    method[k][j] = e;
                }
            }
            // Sort
            printLn("Sorting...");
            time[0] += timing([&]() {
                std::sort(method[0], method[0] + SIZE);
            });
            time[1] += timing([&]() {
                insertionSort(method[1], SIZE);
            });
            time[2] += timing([&]() {
                selectionSort(method[2], SIZE);
            });
            time[3] += timing([&]() {
                heapSort(method[3], SIZE);
            });
            time[4] += timing([&]() {
                quickSort(method[4], 0, SIZE - 1);
            });
            // Quick select
            unsigned k = random->nextInt(0, SIZE - 1);
            time[5] += timing([&]() {
                std::nth_element(method[5], method[5] + k, method[5] + SIZE);
            });
            time[6] += timing([&]() {
                quickSelect(method[6], 0, SIZE - 1, k);
            });
            printLn("Finished.");
            // Correctness check
            bool correct = true;
            for (int i = 0; i < METHOD_NUM - 2; ++i) {
                if (!std::is_sorted(method[i], method[i] + SIZE)) {
                    correct = false;
                    break;
                }
            }
            if (method[5][k] != method[6][k]) {
                correct = false;
            }
            printLn("Correctness check: ", correct ? "pass" : "fail", "\n");
        }
        printLn(CASES, " cases finished.\n");
        printLn("Avg time cost:");
        printLn("       std::sort: ", time[0] / CASES, " ms");
        printLn("  insertion sort: ", time[1] / CASES, " ms");
        printLn("  selection sort: ", time[2] / CASES, " ms");
        printLn("       heap sort: ", time[3] / CASES, " ms");
        printLn("      quick sort: ", time[4] / CASES, " ms");
        printLn("std::nth_element: ", time[5] / CASES, " ms");
        printLn("    quick select: ", time[6] / CASES, " ms");

        // Free resources
        for (int i = 0; i < METHOD_NUM; ++i) {
            delete[] method[i];
            method[i] = nullptr;
        }
    }
    printLn("Benchmark of Sort finished.");
    checkMemoryLeaks();
    return 0;
}
