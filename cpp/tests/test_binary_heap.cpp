#include "catch2/catch_amalgamated.hpp"
#include "jumble/binary_heap.hpp"
#include "jumble/util/random.hpp"

typedef jumble::BinaryHeap<int>::SizeType SizeType;

TEST_CASE("Basic") {
    jumble::BinaryHeap<int> heap;
    REQUIRE(heap.isEmpty());
    REQUIRE(heap.getSize() == (SizeType)0);

    heap.push(50);
    heap.push(20);
    heap.push(30);
    REQUIRE(!heap.isEmpty());
    REQUIRE(heap.getSize() == (SizeType)3);
    REQUIRE(heap.top() == 20);

    heap.pop();
    REQUIRE(heap.top() == 30);

    heap.pop();
    REQUIRE(heap.top() == 50);
    REQUIRE(heap.getSize() == (SizeType)1);

    heap.pop();
    REQUIRE(heap.isEmpty());
    REQUIRE(heap.getSize() == (SizeType)0);
}

TEST_CASE("MinRoot") {
    std::vector<int> ans;
    for (int i = 0; i < 30; ++i) {
        ans.push_back(i);
    }

    std::vector<int> test(ans);
    jumble::Random::getInstance()->shuffle(test.begin(), test.end());

    jumble::BinaryHeap<int> heap(test);
    for (const auto &x : ans) {
        REQUIRE(heap.top() == x);
        heap.pop();
    }
    REQUIRE(heap.isEmpty());
}

TEST_CASE("MaxRoot") {
    std::vector<int> ans;
    for (int i = 29; i >= 0; --i) {
        ans.push_back(i);
    }

    std::vector<int> test(ans);
    jumble::Random::getInstance()->shuffle(test.begin(), test.end());

    jumble::BinaryHeap<int, std::greater<int>> heap(test);
    for (const auto &x : ans) {
        REQUIRE(heap.top() == x);
        heap.pop();
    }
    REQUIRE(heap.isEmpty());
}
