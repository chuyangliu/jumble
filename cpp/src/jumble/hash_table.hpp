#ifndef JUMBLE_HASH_TABLE_HPP_INCLUDED
#define JUMBLE_HASH_TABLE_HPP_INCLUDED

#include "jumble/doubly_linked_list.hpp"
#include <vector>
#include <utility>

JUMBLE_NAMESPACE_BEGIN

/*
A data structure that stores unique elements in no particular order,
and which allows for fast retrieval of individual elements based on
their values.

@param Value    The type of the values stored in the hash table.
@param PredCmp  A binary predicate that checks if two values are equal.
                If PredCmp(a, b) == true, then value 'a' and value 'b' are
                considered equal.
@param Hash  A unary functor that computes the hash value of an element.
*/
template<typename Value, typename PredCmp = std::equal_to<Value>,
         typename Hash = std::hash<Value>>
class HashTable {
public:
    using SizeType = std::size_t;
    using Container = std::vector<DoublyLinkedList<Value>>;

    /*
    Initialize the hash table.

    @param n   Expected buckets amount. In practice, the
               amount of buckets may be greater than 'n'
    @param cmp The comparator
    @param h   The hash function
    */
    explicit HashTable(const SizeType n = MIN_BUCKET,
                       const PredCmp& cmp = PredCmp(),
                       const Hash& h = Hash())
    : size(0), bucketNum(0), predCmp(cmp), hasher(h) {
        rehash(n);
    }

    // Return the amount of elements in the hash table
    SizeType getSize() const noexcept {
        return size;
    }

    // Return true if the hash table has no elements
    bool isEmpty() const noexcept {
        return size == 0;
    }

    // Remove all elements from the hash table
    void clear() {
        size = 0;
        for (SizeType i = 0; i < bucketNum; ++i) {
            buckets[i].clear();
        }
    }

    // Return true if a given value is in the hash table
    bool has(const Value& val) const {
        const auto &list = buckets[hash(val)];
        return list.find(val, predCmp) != -1;
    }

    /*
    Insert a value to the hash table. If the
    value exists, this function does nothing.

    @param val The value to be inserted
    */
    void insert(const Value& val) {
        auto &list = buckets[hash(val)];
        if (list.find(val) == -1) {
            list.insertBack(val);
            ++size;
        }
    }

    /*
    Remove a value from the hash table. If the
    value doesn't exist, this function does nothing.

    @param val The value to be removed
    */
    void remove(const Value& val) {
        auto &list = buckets[hash(val)];
        auto pos = list.find(val);
        if (pos != -1) {
            list.remove(pos);
            --size;
        }
    }

    /*
    Rehash the hash table.

    @param n Expected buckets amount. In practice, the
             amount of buckets may be greater than 'n'.
    */
    void rehash(const SizeType n) {
        size = 0;
        SizeType oldNum = bucketNum;
        bucketNum = MIN_BUCKET;
        while (bucketNum < n) {
            bucketNum <<= 1;
        }
        Container oldBuckets = std::move(buckets);
        buckets.resize(bucketNum);
        for (SizeType i = 0; i < oldNum; ++i) {
            oldBuckets[i].traverse([&](const SizeType pos, const Value& val) {
                UNUSED(pos);
                insert(val);
            });
        }
    }

private:
    /*
    The function to compute the hash value of an element.

    @param val The element
    @return The hash value of the element
    */
    SizeType hash(const Value& val) const {
        return (SizeType)(hasher(val) & (bucketNum - 1));
    }

private:
    static const SizeType MIN_BUCKET = 8;

    SizeType size;
    SizeType bucketNum;

    PredCmp predCmp;
    Hash hasher;

    Container buckets;
};

JUMBLE_NAMESPACE_END

#endif
