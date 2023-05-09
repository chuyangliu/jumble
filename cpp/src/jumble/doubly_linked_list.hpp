#ifndef JUMBLE_DOUBLY_LINKED_LIST_HPP_INCLUDED
#define JUMBLE_DOUBLY_LINKED_LIST_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <functional>
#include <cstddef>

JUMBLE_NAMESPACE_BEGIN

/*
Doubly linked list data structure.

@param Value The type of list node value.
*/
template<typename Value>
class DoublyLinkedList {
private:
    struct Node {
        Value val;
        Node *prev;
        Node *next;

        Node(const Value& v, Node *p = nullptr, Node *n = nullptr) noexcept
        : val(v), prev(p), next(n) {}
    };

public:
    using SizeType = std::size_t;

    // Default ctor
    DoublyLinkedList() noexcept : size(0), head(nullptr), tail(nullptr) {}

    // Forbid copy
    DoublyLinkedList(const DoublyLinkedList&) = delete;
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;

    // Move ctor
    DoublyLinkedList(DoublyLinkedList&& other) noexcept
    : size(other.size), head(other.head), tail(other.tail) {
        other.size = 0;
        other.head = nullptr;
        other.tail = nullptr;
    }

    // Move assignment
    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {
        if (this != &other) {
            clear();
            size = other.size;
            head = other.head;
            tail = other.tail;
            other.size = 0;
            other.head = nullptr;
            other.tail = nullptr;
        }
        return *this;
    }

    // Dtor
    ~DoublyLinkedList() noexcept {
        clear();
    }

    // Return the amount of nodes in the list
    SizeType getSize() const noexcept {
        return size;
    }

    // Return true if the list is empty
    bool isEmpty() const noexcept {
        return size == 0;
    }

    // Clear the content of the list
    void clear() noexcept {
        Node *tmp = head, *del = nullptr;
        while (tmp) {
            del = tmp;
            tmp = tmp->next;
            delete del;
        }
        head = tail = nullptr;
        size = 0;
    }

    /*
    Traverse the list node.

    @param f The function to traverse each list node. Params are:
             @param pos The position of the current traversing node
             @param val The value of the current traversing node
    */
    void traverse(const std::function<
                  void(const SizeType pos, const Value& val)>& f) const {
        SizeType pos = 0;
        for (Node *tmp = head; tmp; tmp = tmp->next, ++pos) {
            f(pos, tmp->val);
        }
    }

    /*
    Find a node in the list.

    @param val The value of the node to be found
    @param cmp The comparator
    @return    The first matching position of the node with value 'val_'.
               If the node does not exist, return -1.
    */
    template<typename PredCmp = std::equal_to<Value>>
    int find(const Value& val, const PredCmp& cmp = PredCmp()) const {
        SizeType pos = 0;
        for (Node *tmp = head; tmp; tmp = tmp->next, ++pos) {
            if (cmp(tmp->val, val)) {
                return (int)pos;
            }
        }
        return -1;
    }

    /*
    Insert a node at a given position. If the position is greater than
    the list's size, the node will be inserted to the back of the list.

    @param pos The position to insert the node
    @param val The value of the node to be inserted
    */
    void insert(SizeType pos, const Value& val) {
        pos = (pos > size ? size : pos);
        if (pos == 0) {
            insertFront(val);
        } else if (pos == size) {
            insertBack(val);
        } else {
            Node *newNode = new Node(val);
            bool prevNode = true;
            Node *bestNode = getBestNode(pos, prevNode);
            if (prevNode) {
                newNode->next = bestNode->next;
                newNode->prev = bestNode;
                if (bestNode->next) {
                    bestNode->next->prev = newNode;
                }
                bestNode->next = newNode;
            } else {
                newNode->next = bestNode;
                newNode->prev = bestNode->prev;
                if (bestNode->prev) {
                    bestNode->prev->next = newNode;
                }
                bestNode->prev = newNode;
            }
            ++size;
        }
    }

    // Insert a node at the front of the list
    void insertFront(const Value& val) {
        Node *newNode = new Node(val, nullptr, head);
        if (head) {
            head->prev = newNode;
        }
        head = newNode;
        if (++size == 1) {
            tail = head;
        }
    }

    // Insert a node at the back of the list
    void insertBack(const Value& val) {
        Node *newNode = new Node(val, tail, nullptr);
        if (tail) {
            tail->next = newNode;
        }
        tail = newNode;
        if (++size == 1) {
            head = tail;
        }
    }

    /*
    Remove a node at a given position. If there is no node
    at the given position, this function does nothing.

    @param pos The position of the node to be removed
    */
    void remove(SizeType pos) noexcept {
        if (pos >= size) {
            return;
        }
        if (pos == 0) {
            removeFront();
        } else if (pos == size - 1) {
            removeBack();
        } else {
            bool prevNode = true;
            Node *del = nullptr;
            Node *bestNode = getBestNode(pos, prevNode);
            if (prevNode) {
                del = bestNode->next;
                bestNode->next = del->next;
                if (del->next) {
                    del->next->prev = bestNode;
                }
            } else {  // bestNode->prev is not null
                del = bestNode;
                del->prev->next = del->next;
                if (del->next) {
                    del->next->prev = del->prev;
                }
            }
            delete del;
            --size;
        }
    }

    // Remove the first node of the list
    void removeFront() noexcept {
        if (isEmpty()) {
            return;
        }
        Node *del = head;
        head = head->next;
        if (--size == 0) {
            tail = head;
        } else {
            head->prev = nullptr;
        }
        delete del;
    }

    // Remove the last node of the list
    void removeBack() noexcept {
        if (isEmpty()) {
            return;
        }
        Node *del = tail;
        tail = tail->prev;
        if (--size == 0) {
            head = tail;
        } else {
            tail->next = nullptr;
        }
        delete del;
    }

    /*
    Sort the list nodes.

    @param cmp A binary predicate to compare two node values. The sorting
               algorithm ensures that after its execution, for each node A
               and its next node B in the list, A.value == B.value OR
               cmp(A.value, B.value) == true.
    */
    template<typename PredCmp = std::less<Value>>
    void sort(const PredCmp& cmp = PredCmp()) {
        head = mergeSort(head, size, cmp);
    }

private:
    /*
    Get a best node to perform inserting operation at a given position.
    The function is available only when the list is not empty.

    @param pos      The given position. Its values must lie in the interval (0, size).
    @param prevNode If true, the returned node is previous to the node at
                    the given position. If false, the returned node is the
                    node at the given position.
    @return         The pointer to the best node.
    */
    Node* getBestNode(const SizeType& pos, bool& prevNode) const noexcept {
        Node *res = nullptr;
        SizeType distToHead = pos - 1, distToTail = size - pos - 1;
        if (distToHead < distToTail) {
            res = head;
            for (SizeType i = 0; i < distToHead; ++i) {
                res = res->next;
            }
            prevNode = true;
        } else {
            res = tail;
            for (SizeType i = 0; i < distToTail; ++i) {
                res = res->prev;
            }
            prevNode = false;
        }
        return res;
    }

    /*
    Running a merge sort procedure of the list.

    @param h     The head of the list
    @param size_ The size of the list
    @param cmp   A binary predicate to compare two node values
    @return      The head pointer of the sorted list
    */
    template<typename PredCmp>
    Node* mergeSort(Node* h, const SizeType size_, const PredCmp& cmp) {
        if (size_ < 2) {
            return h;
        }

        // Find middle position
        Node *mid = h;
        SizeType size1 = size_ >> 1, size2 = size_ - size1;
        for (SizeType i = 0; i < size1 - 1; ++i) {
            mid = mid->next;
        }

        // Sort each half of the list
        Node *h2 = mergeSort(mid->next, size2, cmp);
        Node *h1 = mergeSort(h, size1, cmp);

        // Merge h1 and h2(they are both sorted and non-empty)
        SizeType cnt = 0;
        Node *newHead = h1;
        for (bool initLoop = true; ; initLoop = false) {
            if (cmp(h2->val, h1->val)) {
                if (initLoop) {
                    newHead = h2;
                }
                Node *h2Prev = h2->prev, *h2Next = h2->next, *h1Prev = h1->prev;
                h2Prev->next = h2Next;
                if (h2Next) {
                    h2Next->prev = h2->prev;
                } else {  // New tail
                    tail = h2Prev;
                }
                if (h1Prev) {
                    h1Prev->next = h2;
                }
                h2->prev = h1Prev;
                h2->next = h1;
                h1->prev = h2;
                h2 = h2Next;
                if (++cnt == size2) {
                    break;
                }
            } else {
                h1 = h1->next;
                if (h1 == h2) {
                    break;
                }
            }
        }
        return newHead;
    }

private:
    SizeType size;
    Node *head;
    Node *tail;
};

JUMBLE_NAMESPACE_END

#endif
