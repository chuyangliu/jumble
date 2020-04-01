#ifndef JUMBLE_BINARY_HEAP_HPP_INCLUDED
#define JUMBLE_BINARY_HEAP_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <vector>
#include <functional>

JUMBLE_NAMESPACE_BEGIN

/*
A heap data structure taking the form of a complete binary tree.

@param Value    The type of the value stored in the heap
@param PredCmp  The type of the binary predCmpicate to arrange tree the nodes.
                The heap ensures that for each node N in the tree,
                N.parent.value == N.value OR PredCmp(N.parent.value, N.value) == true.
                By default it uses operator '>=' as the predCmpicate, which makes it
                a min-root heap.
*/
template<typename Value, typename PredCmp = std::less<Value>>
class BinaryHeap {
public:
    using Container = std::vector<Value>;
    using SizeType = typename Container::size_type;

    // Default ctor
    BinaryHeap(const PredCmp& cmp = PredCmp())
    : size(0), predCmp(cmp), tree(1) {}

    // Build heap with a given array of values
    explicit BinaryHeap(const Container& vals, const PredCmp& cmp = PredCmp())
    : size(vals.size()), predCmp(cmp), tree(vals) {
        tree.push_back(tree[0]);  // tree[0] has no use, index begins at 1
        for (SizeType i = (size >> 1); i > 0; --i) {
            Value val = tree[i];  // Make a copy
            percolateDown(i, val);
        }
    }

    // Return the amount of elements in the heap
    SizeType getSize() const noexcept {
        return size;
    }

    // Return true if the heap has no elements
    bool isEmpty() const noexcept {
        return size == 0;
    }

    // Clear the elements in the heap
    void clear() noexcept {
        size = 0;
    }

    // Insert a new element to the heap
    void push(const Value& val) {
        if (size + 1 == tree.size()) {  // The tree array is full
            tree.resize(tree.size() << 1);  // Expand space
        }
        // The new element percolates up in the heap
        percolateUp(++size, val);
    }

    /*
    Return the root element of the heap.
    Precondition: The heap is non-empty.
    */
    Value top() const {
        return tree[1];
    }

    /*
    Remove the root element of the heap and maintain the nodes order.
    Precondition: The heap is non-empty.
    */
    void pop() {
        // The empty hole (root node) percolates down
        percolateDown(1, tree[size--]);
    }

private:
    void percolateUp(const SizeType start, const Value& val) {
        auto i = start;
        for (; i != 1 && predCmp(val, tree[i >> 1]); i >>= 1) {
            tree[i] = tree[i >> 1];
        }
        tree[i] = val;
    }

    void percolateDown(const SizeType start, const Value& val) {
        SizeType i = start, child;
        for (; (i << 1) <= size; i = child) {
            child = i << 1;
            if (child != size && predCmp(tree[child + 1], tree[child])) {
                ++child;
            }
            if (predCmp(val, tree[child])) {
                break;
            } else {
                tree[i] = tree[child];
            }
        }
        tree[i] = val;
    }

private:
    SizeType size;
    PredCmp predCmp;

    /*
    Store the complete binary tree in an array named 'tree'.
    The first element is stored at tree[1]. The value of
    tree[0] has no use.

    For each node i in the tree:
    1. tree[2 * i] is the node value of its left child.
    2. tree[2 * i + 1] is the node value of its right child.
    3. tree[floor(i / 2)] is the node value if its parent node.
    */
    Container tree;
};

JUMBLE_NAMESPACE_END

#endif
