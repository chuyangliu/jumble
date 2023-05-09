#include "catch2/catch_amalgamated.hpp"
#include "jumble/sort.hpp"
#include "jumble/util/random.hpp"
#include <algorithm>

TEST_CASE("InsertionSort") {
    jumble::Random *random = jumble::Random::getInstance();
    const int n = 3000;
    int arr1[n], arr2[n];
    for (int i = 0; i < n; ++i) {
        arr1[i] = arr2[i] = random->nextInt(1, 10000);
    }
    std::sort(arr1, arr1 + n, std::greater<int>());
    jumble::insertionSort<int, std::greater<int>>(arr2, n);
    REQUIRE(std::is_sorted(arr1, arr1 + n, std::greater<int>()));
    REQUIRE(std::is_sorted(arr2, arr2 + n, std::greater<int>()));
    for (int i = 0; i < n; ++i) {
        REQUIRE(arr1[i] == arr2[i]);
    }
}

TEST_CASE("SelectionSort") {
    jumble::Random *random = jumble::Random::getInstance();
    const int n = 3000;
    int arr1[n], arr2[n];
    for (int i = 0; i < n; ++i) {
        arr1[i] = arr2[i] = random->nextInt(1, 10000);
    }
    std::sort(arr1, arr1 + n, std::greater<int>());
    jumble::selectionSort<int, std::greater<int>>(arr2, n);
    REQUIRE(std::is_sorted(arr1, arr1 + n, std::greater<int>()));
    REQUIRE(std::is_sorted(arr2, arr2 + n, std::greater<int>()));
    for (int i = 0; i < n; ++i) {
        REQUIRE(arr1[i] == arr2[i]);
    }
}

TEST_CASE("HeapSort") {
    jumble::Random *random = jumble::Random::getInstance();
    const int n = 3000;
    int arr1[n], arr2[n];
    for (int i = 0; i < n; ++i) {
        arr1[i] = arr2[i] = random->nextInt(1, 10000);
    }
    std::sort(arr1, arr1 + n, std::greater<int>());
    jumble::heapSort<int, std::greater<int>>(arr2, n);
    REQUIRE(std::is_sorted(arr1, arr1 + n, std::greater<int>()));
    REQUIRE(std::is_sorted(arr2, arr2 + n, std::greater<int>()));
    for (int i = 0; i < n; ++i) {
        REQUIRE(arr1[i] == arr2[i]);
    }
}

TEST_CASE("QuickSort") {
    jumble::Random *random = jumble::Random::getInstance();
    const int n = 3000;
    int arr1[n], arr2[n];
    for (int i = 0; i < n; ++i) {
        arr1[i] = arr2[i] = random->nextInt(1, 10000);
    }
    std::sort(arr1, arr1 + n, std::greater<int>());
    jumble::quickSort<int, std::greater<int>>(arr2, 0, n - 1);
    REQUIRE(std::is_sorted(arr1, arr1 + n, std::greater<int>()));
    REQUIRE(std::is_sorted(arr2, arr2 + n, std::greater<int>()));
    for (int i = 0; i < n; ++i) {
        REQUIRE(arr1[i] == arr2[i]);
    }
}

TEST_CASE("QuickSelect") {
    jumble::Random *random = jumble::Random::getInstance();
    const int n = 3000;
    for (int i = 0; i < 5; ++i) {
        int arr1[n], arr2[n];
        for (int j = 0; j < n; ++j) {
            arr1[j] = arr2[j] = random->nextInt(1, 10000);
        }
        unsigned k = random->nextInt(0, n - 1);
        std::nth_element(arr1, arr1 + k, arr1 + n, std::greater<int>());
        jumble::quickSelect<int, std::greater<int>>(arr2, 0, n - 1, k);
        REQUIRE(arr1[k] == arr2[k]);
    }
}
