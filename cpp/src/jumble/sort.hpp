#ifndef JUMBLE_SORT_HPP_INCLUDED
#define JUMBLE_SORT_HPP_INCLUDED

#include "jumble/binary_heap.hpp"
#include <cstddef>
#include <utility>

JUMBLE_NAMESPACE_BEGIN

/*
The insertion sort algorithm to sort an unordered array.

@param arr The array to be sorted
@param n   The amount of elements in the array
@param cmp The comparator
*/
template<typename Value, typename PredCmp = std::less<Value>>
void insertionSort(Value *const arr, const std::size_t n, const PredCmp& cmp = PredCmp()) {
    for (std::size_t i = 1; i < n; ++i) {
        if (!cmp(arr[i - 1], arr[i])) {
            Value tmp = arr[i];
            std::size_t j = i;
            while (j >= 1 && cmp(tmp, arr[j - 1])) {
                arr[j] = arr[j - 1];
                --j;
            }
            arr[j] = tmp;
        }
    }
}

/*
The selection sort algorithm to sort an unordered array.

@param arr The array to be sorted
@param n   The amount of elements in the array
@param cmp The comparator
*/
template<typename Value, typename PredCmp = std::less<Value>>
void selectionSort(Value *const arr, const std::size_t n, const PredCmp& cmp = PredCmp()) {
    for (std::size_t i = 0; i < n - 1; ++i) {
        std::size_t minOrMax = i;
        for (std::size_t k = i; k < n; ++k) {
            if (cmp(arr[k], arr[minOrMax])) {
                minOrMax = k;
            }
        }
        if (minOrMax != i) {
            std::swap(arr[i], arr[minOrMax]);
        }
    }
}

/*
The heapsort algorithm to sort an unordered array.

@param arr The array to be sorted
@param n   The amount of elements in the array
@param cmp The comparator
*/
template<typename Value, typename PredCmp = std::less<Value>>
void heapSort(Value *const arr, const std::size_t n, const PredCmp& cmp = PredCmp()) {
    BinaryHeap<Value, PredCmp> heap(cmp);
    for (std::size_t i = 0; i < n; ++i) {
        heap.push(arr[i]);
    }
    for (std::size_t i = 0; i < n; ++i) {
        arr[i] = heap.top();
        heap.pop();
    }
}

/*
Return the pivot element in an array. This function is used in quickSort()
and quickSelect(). The algorithm is based on the median-of-three pivoting.
It ensures that after its execution, PredCmp(arr[left], arr[mid]) == true AND
PredCmp(arr[mid], arr[right]) == true. The pivot element will be stored at
arr[right - 1].

@param a     The array to be processed
@param left  The beginning index of the array to be processed
@param right The ending index of the array to be processed
@param cmp   The comparator
@return      The pivot element
*/
template<typename Value, typename PredCmp = std::less<Value>>
Value pivot(Value *const arr,
            const std::size_t left,
            const std::size_t right,
            const PredCmp& cmp = PredCmp()) {
    std::size_t mid = (left + right) / 2;
    if (cmp(arr[mid], arr[left])) {
        std::swap(arr[mid], arr[left]);
    }
    if (cmp(arr[right], arr[left])) {
        std::swap(arr[right], arr[left]);
    }
    if (cmp(arr[right], arr[mid])) {
        std::swap(arr[right], arr[mid]);
    }
    std::swap(arr[mid], arr[right - 1]);  // Store pivot at arr[right - 1]
    return arr[right - 1];
}

/*
The quicksort algorithm to sort an unordered array.

@param arr   The array to be sorted
@param left  The beginning index of the array to be sorted
@param right The ending index of the array to be sorted
@param cmp   The comparator
*/
template<typename Value, typename PredCmp = std::less<Value>>
void quickSort(Value *const arr,
               const std::size_t left,
               const std::size_t right,
               const PredCmp& cmp = PredCmp()) {
    if (left + 10 <= right) {
        Value p = pivot<Value, PredCmp>(arr, left, right);
        std::size_t i = left, j = right - 1;
        while (true) {
            while (cmp(arr[++i], p)) {}
            while (cmp(p, arr[--j])) {}
            if (i < j) {
                std::swap(arr[i], arr[j]);
            } else {
                break;
            }
        }
        std::swap(arr[i], arr[right - 1]);  // Restore pivot
        quickSort<Value, PredCmp>(arr, left, i - 1, cmp);
        quickSort<Value, PredCmp>(arr, i + 1, right, cmp);
    } else {
        // Use insertion sort for small array
        insertionSort<Value, PredCmp>(arr + left, right - left + 1, cmp);
    }
}

/*
The quickselect algorithm to find the kth smallest/biggest element
in an unordered array. After its execution, the kth element will be
stored at arr[k].

@param arr   The array to be processed
@param left  The beginning index of the array to be processed
@param right The ending index of the array to be processed
@param k     The kth element to find
@param cmp   The comparator
*/
template<typename Value, typename PredCmp = std::less<Value>>
void quickSelect(Value *const arr,
                 const std::size_t left,
                 const std::size_t right,
                 const std::size_t k,
                 const PredCmp& cmp = PredCmp()) {
    if (left + 10 <= right) {
        Value p = pivot<Value, PredCmp>(arr, left, right);
        std::size_t i = left, j = right - 1;
        while (true) {
            while (cmp(arr[++i], p)) {}
            while (cmp(p, arr[--j])) {}
            if (i < j) {
                std::swap(arr[i], arr[j]);
            } else {
                break;
            }
        }
        std::swap(arr[i], arr[right - 1]);  // Restore pivot
        if (k < i) {
            quickSelect<Value, PredCmp>(arr, left, i - 1, k, cmp);
        } else if (k > i) {
            quickSelect<Value, PredCmp>(arr, i + 1, right, k, cmp);
        }
    } else {
        // Use insertion sort for small array
        insertionSort<Value, PredCmp>(arr + left, right - left + 1, cmp);
    }
}

JUMBLE_NAMESPACE_END

#endif
