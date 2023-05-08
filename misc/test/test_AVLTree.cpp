#include "gtest/gtest.h"
#include "tastylib/AVLTree.h"

using tastylib::AVLTree;

typedef AVLTree<int>::SizeType SizeType;

TEST(AVLTreeTest, Basic) {
    AVLTree<int> tree;
    tree.clear();
    tree.clear();
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.preorder(), "{}");
    EXPECT_EQ(tree.inorder(), "{}");
    EXPECT_EQ(tree.postorder(), "{}");
    tree.insert(1);
    tree.insert(2);
    tree.insert(3);
    tree.insert(3);
    EXPECT_FALSE(tree.isEmpty());
    EXPECT_EQ(tree.getSize(), (SizeType)4);
    EXPECT_TRUE(tree.has(3));
    EXPECT_EQ(tree.preorder(), "{2, 1, 3, 3}");
    EXPECT_EQ(tree.inorder(), "{1, 2, 3, 3}");
    EXPECT_EQ(tree.postorder(), "{1, 3, 3, 2}");
    tree.remove(3);
    EXPECT_TRUE(tree.has(1));
    EXPECT_FALSE(tree.has(3));
    EXPECT_EQ(tree.getSize(), (SizeType)2);
    EXPECT_EQ(tree.preorder(), "{2, 1}");
    tree.clear();
    tree.clear();
    EXPECT_FALSE(tree.has(1));
    EXPECT_TRUE(tree.isEmpty());
    EXPECT_EQ(tree.getSize(), (SizeType)0);
}

TEST(AVLTreeTest, CopyControl) {
    AVLTree<int> tree1;
    tree1.insert(1);
    tree1.insert(2);
    tree1.insert(3);
    tree1.insert(3);

    AVLTree<int> tree2(std::move(tree1));
    EXPECT_TRUE(tree1.isEmpty());
    EXPECT_EQ(tree2.preorder(), "{2, 1, 3, 3}");
    EXPECT_EQ(tree2.inorder(), "{1, 2, 3, 3}");
    EXPECT_EQ(tree2.postorder(), "{1, 3, 3, 2}");

    tree1 = std::move(tree2);
    EXPECT_TRUE(tree2.isEmpty());
    EXPECT_EQ(tree1.preorder(), "{2, 1, 3, 3}");
    EXPECT_EQ(tree1.inorder(), "{1, 2, 3, 3}");
    EXPECT_EQ(tree1.postorder(), "{1, 3, 3, 2}");
}

TEST(AVLTreeTest, Insert) {
    AVLTree<int> tree;
    tree.insert(3);
    tree.insert(2);
    tree.insert(1);
    tree.insert(4);
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(16);
    tree.insert(15);
    tree.insert(14);
    tree.insert(13);
    tree.insert(12);
    tree.insert(11);
    tree.insert(10);
    tree.insert(8);
    tree.insert(9);
    EXPECT_EQ(tree.preorder(), "{7, 4, 2, 1, 3, 6, 5, 13, 11, 9, 8, 10, 12, 15, 14, 16}");
    EXPECT_EQ(tree.inorder(), "{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}");
    EXPECT_EQ(tree.postorder(), "{1, 3, 2, 5, 6, 4, 8, 10, 9, 12, 11, 14, 16, 15, 13, 7}");
}

TEST(AVLTreeTest, Remove) {
    AVLTree<int> tree;
    // Single rotate with right
    tree.insert(5);
    tree.insert(10);
    tree.insert(15);
    tree.insert(20);
    tree.remove(5);
    EXPECT_EQ(tree.preorder(), "{15, 10, 20}");
    EXPECT_EQ(tree.inorder(), "{10, 15, 20}");
    EXPECT_EQ(tree.postorder(), "{10, 20, 15}");
    // Single rotate with left
    tree.insert(5);
    tree.remove(20);
    EXPECT_EQ(tree.preorder(), "{10, 5, 15}");
    EXPECT_EQ(tree.inorder(), "{5, 10, 15}");
    EXPECT_EQ(tree.postorder(), "{5, 15, 10}");
    // Double rotate with right
    tree.insert(2);
    tree.insert(16);
    tree.insert(8);
    tree.insert(6);
    tree.remove(2);
    EXPECT_EQ(tree.preorder(), "{10, 6, 5, 8, 15, 16}");
    EXPECT_EQ(tree.inorder(), "{5, 6, 8, 10, 15, 16}");
    EXPECT_EQ(tree.postorder(), "{5, 8, 6, 16, 15, 10}");
    // Double rotate with left
    tree.insert(11);
    tree.insert(13);
    tree.remove(16);
    EXPECT_EQ(tree.preorder(), "{10, 6, 5, 8, 13, 11, 15}");
    EXPECT_EQ(tree.inorder(), "{5, 6, 8, 10, 11, 13, 15}");
    EXPECT_EQ(tree.postorder(), "{5, 8, 6, 11, 15, 13, 10}");
    // Remove remnants
    tree.remove(10);
    tree.remove(13);
    tree.remove(15);
    EXPECT_EQ(tree.preorder(), "{6, 5, 11, 8}");
    EXPECT_EQ(tree.inorder(), "{5, 6, 8, 11}");
    EXPECT_EQ(tree.postorder(), "{5, 8, 11, 6}");
    tree.remove(11);
    tree.remove(999);  // Not exist
    EXPECT_EQ(tree.preorder(), "{6, 5, 8}");
    EXPECT_EQ(tree.inorder(), "{5, 6, 8}");
    EXPECT_EQ(tree.postorder(), "{5, 8, 6}");
    EXPECT_EQ(tree.getSize(), (SizeType)3);
    tree.remove(6);
    tree.remove(5);
    tree.remove(8);
    tree.remove(999);  // Not exist
    EXPECT_TRUE(tree.isEmpty());
}

TEST(AVLTreeTest, DescendingOrder) {
    AVLTree<int, std::greater<int>> tree;
    tree.insert(20);
    tree.insert(15);
    tree.insert(10);
    tree.insert(30);
    EXPECT_EQ(tree.preorder(), "{15, 20, 30, 10}");
    EXPECT_EQ(tree.inorder(), "{30, 20, 15, 10}");
    EXPECT_EQ(tree.postorder(), "{30, 20, 10, 15}");
    tree.insert(17);
    tree.insert(18);
    EXPECT_EQ(tree.preorder(), "{17, 20, 30, 18, 15, 10}");
    EXPECT_EQ(tree.inorder(), "{30, 20, 18, 17, 15, 10}");
    EXPECT_EQ(tree.postorder(), "{30, 18, 20, 10, 15, 17}");
    tree.remove(15);
    tree.remove(10);
    EXPECT_EQ(tree.preorder(), "{20, 30, 17, 18}");
    EXPECT_EQ(tree.inorder(), "{30, 20, 18, 17}");
    EXPECT_EQ(tree.postorder(), "{30, 18, 17, 20}");
    tree.remove(30);
    EXPECT_EQ(tree.preorder(), "{18, 20, 17}");
    EXPECT_EQ(tree.inorder(), "{20, 18, 17}");
    EXPECT_EQ(tree.postorder(), "{20, 17, 18}");
    tree.remove(18);
    tree.remove(20);
    tree.remove(17);
    EXPECT_TRUE(tree.isEmpty());
}
