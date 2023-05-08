#include "gtest/gtest.h"
#include "tastylib/util/random.h"
#include "tastylib/BinaryHeap.h"

using tastylib::Random;
using tastylib::BinaryHeap;

typedef BinaryHeap<int>::SizeType SizeType;

TEST(BinaryHeapTest, Basic) {
    BinaryHeap<int> heap;
    EXPECT_TRUE(heap.isEmpty());
    EXPECT_EQ(heap.getSize(), (SizeType)0);
    heap.push(50);
    heap.push(20);
    heap.push(30);
    EXPECT_FALSE(heap.isEmpty());
    EXPECT_EQ(heap.getSize(), (SizeType)3);
    EXPECT_EQ(heap.top(), 20);
    heap.pop();
    EXPECT_EQ(heap.top(), 30);
    heap.pop();
    EXPECT_EQ(heap.top(), 50);
    EXPECT_EQ(heap.getSize(), (SizeType)1);
    heap.pop();
    EXPECT_TRUE(heap.isEmpty());
    EXPECT_EQ(heap.getSize(), (SizeType)0);
}

TEST(BinaryHeapTest, MinRoot) {
    std::vector<int> ans;
    for (int i = 0; i < 30; ++i) {
        ans.push_back(i);
    }
    std::vector<int> test(ans);
    Random::getInstance()->shuffle(test.begin(), test.end());
    BinaryHeap<int> heap(test);
    for (const auto &x : ans) {
        EXPECT_EQ(heap.top(), x);
        heap.pop();
    }
    EXPECT_TRUE(heap.isEmpty());
}

TEST(BinaryHeapTest, MaxRoot) {
    std::vector<int> ans;
    for (int i = 29; i >= 0; --i) {
        ans.push_back(i);
    }
    std::vector<int> test(ans);
    Random::getInstance()->shuffle(test.begin(), test.end());
    BinaryHeap<int, std::greater<int>> heap(test);
    for (const auto &x : ans) {
        EXPECT_EQ(heap.top(), x);
        heap.pop();
    }
    EXPECT_TRUE(heap.isEmpty());
}
