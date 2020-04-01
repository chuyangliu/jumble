#ifndef JUMBLE_AVL_TREE_HPP_INCLUDED
#define JUMBLE_AVL_TREE_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <functional>
#include <string>
#include <sstream>
#include <utility>
#include <cstddef>

JUMBLE_NAMESPACE_BEGIN

/*
A self-balancing binary search tree.

@param Value   The type of the values stored in the tree nodes
@param PredCmp A binary predicate to arrange the tree nodes.
               The tree ensures that for each node N,
               PredCmp(N.leftchild.value, N.value) == true AND
               PredCmp(N.value, N.rightchild.value) == true.
               (must ensure strict weak ordering and should not
                throw any exceptions when copy or move)
*/
template<typename Value, typename PredCmp = std::less<Value>>
class AVLTree {
private:
    struct Node {
        Value val;
        Node *left;
        Node *right;
        int height;

        Node(const Value& v, Node *const l = nullptr, Node *const r = nullptr) noexcept
        : val(v), left(l), right(r), height(0) {}
    };

public:
    using SizeType = std::size_t;

    // Default ctor
    AVLTree(const PredCmp& cmp = PredCmp()) noexcept
    : root(nullptr), size(0), predCmp(cmp) {}

    // Forbid copy
    AVLTree(const AVLTree&) = delete;
    AVLTree& operator=(const AVLTree&) = delete;

    // Move ctor
    AVLTree(AVLTree&& other) noexcept
    : root(other.root), size(other.size), predCmp(std::move(other.predCmp)) {
        other.root = nullptr;
        other.size = 0;
    }

    // Move assignment
    AVLTree& operator=(AVLTree&& other) noexcept {
        if (this != &other) {
            clear();
            root = other.root;
            size = other.size;
            predCmp = std::move(other.predCmp);
            other.root = nullptr;
            other.size = 0;
        }
        return *this;
    }

    // Dtor
    ~AVLTree() noexcept {
        clear();
    }

    // Return true if there are no elements in the tree
    bool isEmpty() const noexcept {
        return size == 0;
    }

    // Return the amount of elements in the tree
    SizeType getSize() const noexcept {
        return size;
    }

    // Remove all nodes and free the resources
    void clear() noexcept {
        release(root);
        size = 0;
        root = nullptr;
    }

    // Return true if a given value is in the tree
    bool has(const Value& val) const {
        return !!find(val, root);
    }

    // Insert a value
    void insert(const Value& val) {
        root = insertBalance(val, root);
        ++size;
    }

    // Remove all nodes with a given value in the tree
    void remove(const Value& val) {
        while (has(val)) {
            root = removeBalance(val, root);
            --size;
        }
    }

    /*
    Return a string representation of the tree node values traversed
    in postorder. The value type must overload operator '<<'.
    */
    std::string postorder() const {
        bool first = true;
        std::ostringstream oss;
        oss << "{";
        traversePostorder(root, [&](const Value& val) {
            if (!first) {
                oss << ", ";
            }
            first = false;
            oss << val;
        });
        oss << "}";
        return oss.str();
    }

    /*
    Return a string representation of the tree node values traversed
    in inorder. The value type must overload operator '<<'.
    */
    std::string inorder() const {
        bool first = true;
        std::ostringstream oss;
        oss << "{";
        traverseInorder(root, [&](const Value& val) {
            if (!first) {
                oss << ", ";
            }
            first = false;
            oss << val;
        });
        oss << "}";
        return oss.str();
    }

    /*
    Return a string representation of the tree node values traversed
    in preorder. The value type must overload operator '<<'.
    */
    std::string preorder() const {
        bool first = true;
        std::ostringstream oss;
        oss << "{";
        traversePreorder(root, [&](const Value& val) {
            if (!first) {
                oss << ", ";
            }
            first = false;
            oss << val;
        });
        oss << "}";
        return oss.str();
    }

private:
    // Equality check ensured by strict weak ordering
    bool isEqual(const Value& lhs, const Value& rhs) const {
        return !predCmp(lhs, rhs) && !predCmp(rhs, lhs);
    }

    // Remove all nodes of a tree and free the resources.
    void release(Node *const root) noexcept {
        if (root) {
            release(root->left);
            release(root->right);
            delete root;
        }
    }

    /*
    Find a node that has a given value in a tree.

    @param val The given value
    @param r   The root of the tree
    @return    The pointer of the node that has the given value.
               Return nullptr if the node does not exist.
    */
    Node* find(const Value& val, Node *const r) const {
        if (!r || isEqual(val, r->val)) {
            return r;
        } else if (predCmp(val, r->val)) {
            return find(val, r->left);
        } else {
            return find(val, r->right);
        }
    }

    /*
    Traverse a tree in postorder.

    @param r The root of the tree.
    @param f The function to manipulate each node value.
    */
    void traversePostorder(const Node *const r,
                           const std::function<void(const Value&)>& f) const {
        if (r) {
            traversePostorder(r->left, f);
            traversePostorder(r->right, f);
            f(r->val);
        }
    }

    /*
    Traverse a tree in inorder.

    @param r The root of the tree.
    @param f The function to manipulate each node value.
    */
    void traverseInorder(const Node *const r,
                         const std::function<void(const Value&)>& f) const {
        if (r) {
            traverseInorder(r->left, f);
            f(r->val);
            traverseInorder(r->right, f);
        }
    }

    /*
    Traverse a tree in preorder.

    @param r The root of the tree.
    @param f The function to manipulate each node value.
    */
    void traversePreorder(const Node *const r,
                          const std::function<void(const Value&)>& f) const {
        if (r) {
            f(r->val);
            traversePreorder(r->left, f);
            traversePreorder(r->right, f);
        }
    }

    /*
    Insert a value to a tree and maintain the balance.

    @param val The value to be inserted
    @param r   The root of the tree
    @return    The new root of the tree that has been inserted
    */
    Node* insertBalance(const Value& val, Node* r) {
        if (!r) {
            r = new Node(val);
        } else if (predCmp(val, r->val)) {
            r->left = insertBalance(val, r->left);
            if (height(r->left) - height(r->right) == 2) {
                if (predCmp(val, r->left->val)) {
                    r = rotateSingleLeft(r);
                } else {
                    r = rotateDoubleLeft(r);
                }
            }
        } else {
            r->right = insertBalance(val, r->right);
            if (height(r->right) - height(r->left) == 2) {
                if (predCmp(val, r->right->val)) {
                    r = rotateDoubleRight(r);
                } else {
                    r = rotateSingleRight(r);
                }
            }
        }
        updateHeight(r);
        return r;
    }

    /*
    Remove a value from a tree and maintain the balance.
    Precondition: the value must exist in the tree.

    @param val The value to be inserted
    @param r   The root of the tree
    @return    The new root of the tree that has been removed
    */
    Node* removeBalance(const Value& val, Node* r) {
        if (isEqual(val, r->val)) {
            if (r->left && r->right) {  // Two child
                Node *node = leftmost(r->right);
                r->val = node->val;
                r->right = removeBalance(node->val, r->right);
            } else {  // One or zero child
                Node *del = r;
                r = !r->left ? r->right : r->left;
                delete del;
            }
        } else if (predCmp(val, r->val)) {
            r->left = removeBalance(val, r->left);
        } else {
            r->right = removeBalance(val, r->right);
        }
        // Maintain balance
        if (r) {
            int d = height(r->left) - height(r->right);
            if (d == 2) {
                if (height(r->left->left) >= height(r->left->right)) {
                    r = rotateSingleLeft(r);
                } else {
                    r = rotateDoubleLeft(r);
                }
            } else if (d == -2) {
                if (height(r->right->right) >= height(r->right->left)) {
                    r = rotateSingleRight(r);
                } else {
                    r = rotateDoubleRight(r);
                }
            }
            updateHeight(r);
        }
        return r;
    }

    // Return the leftmost node in a tree
    Node* leftmost(Node *const root) const noexcept {
        if (!root || !root->left) {
            return root;
        } else {
            return leftmost(root->left);
        }
    }

    // Return the height of a tree. (height of the empty tree equals -1)
    int height(const Node *const root) const noexcept {
        return root ? root->height : -1;
    }

    // Update the height of a tree
    void updateHeight(Node *const root) noexcept {
        int lh = height(root->left), rh = height(root->right);
        root->height = 1 + (lh > rh ? lh : rh);
    }

    /*
    Single rotation with left when inserting
    into the left subtree of the left child.

    @param r The root of the tree
    @return  The new root of the rotated tree
    */
    Node* rotateSingleLeft(Node *const r) noexcept {
        Node *leftChild = r->left;
        r->left = leftChild->right;
        leftChild->right = r;
        updateHeight(r);
        updateHeight(leftChild);
        return leftChild;
    }

    /*
    Single rotation with right when inserting
    into the right subtree of the right child.

    @param r The root of the tree
    @return  The new root of the rotated tree
    */
    Node* rotateSingleRight(Node *const r) noexcept {
        Node *rightChild = r->right;
        r->right = rightChild->left;
        rightChild->left = r;
        updateHeight(r);
        updateHeight(rightChild);
        return rightChild;
    }

    /*
    Double rotation with left when inserting
    into the right subtree of the left child.

    @param r The root of the tree
    @return  The new root of the rotated tree
    */
    Node* rotateDoubleLeft(Node *const r) noexcept {
        r->left = rotateSingleRight(r->left);
        return rotateSingleLeft(r);
    }

    /*
    Double rotation with right when inserting
    into the left subtree of the right child.

    @param r The root of the tree
    @return  The new root of the rotated tree
    */
    Node* rotateDoubleRight(Node *const r) noexcept {
        r->right = rotateSingleLeft(r->right);
        return rotateSingleRight(r);
    }

private:
    Node *root;
    SizeType size;

    PredCmp predCmp;
};

JUMBLE_NAMESPACE_END

#endif
