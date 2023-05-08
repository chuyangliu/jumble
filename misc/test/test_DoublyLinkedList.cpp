#include "gtest/gtest.h"
#include "tastylib/DoublyLinkedList.h"
#include <string>
#include <utility>

using tastylib::DoublyLinkedList;

typedef DoublyLinkedList<int>::SizeType SizeType;

template<typename T>
std::string getListContent(const DoublyLinkedList<T>& list) {
    std::string str;
    list.traverse([&](const SizeType pos, const T& val) {
        UNUSED(pos);
        str += std::to_string(val);
    });
    return str;
}

TEST(DoublyLinkedListTest, Basic) {
    DoublyLinkedList<int> list;
    list.removeFront();
    list.removeBack();
    EXPECT_TRUE(list.isEmpty());
    list.insertBack(1);
    EXPECT_EQ(getListContent(list), "1");
    list.insertFront(2);
    EXPECT_EQ(getListContent(list), "21");
    list.insert(1, 3);
    EXPECT_EQ(getListContent(list), "231");
    list.insert(3, 4);
    EXPECT_EQ(getListContent(list), "2314");
    list.sort();
    EXPECT_EQ(getListContent(list), "1234");
    int p1 = list.find(3);
    EXPECT_EQ(p1, 2);
    list.remove(p1);
    EXPECT_EQ(getListContent(list), "124");
    list.removeFront();
    EXPECT_EQ(getListContent(list), "24");
    list.removeBack();
    EXPECT_EQ(getListContent(list), "2");
    int p2 = list.find(3);
    EXPECT_EQ(p2, -1);
    EXPECT_EQ(list.getSize(), (SizeType)1);
    list.clear();
    EXPECT_TRUE(list.isEmpty());
    EXPECT_EQ(list.getSize(), (SizeType)0);
}

TEST(DoublyLinkedListTest, CopyControl) {
    DoublyLinkedList<int> list1;
    list1.insertBack(1);
    list1.insertBack(2);

    DoublyLinkedList<int> list2(std::move(list1));
    EXPECT_TRUE(list1.isEmpty());
    EXPECT_EQ(getListContent(list2), "12");

    list1 = std::move(list2);
    EXPECT_TRUE(list2.isEmpty());
    EXPECT_EQ(getListContent(list1), "12");
}

TEST(DoublyLinkedListTest, Insert) {
    DoublyLinkedList<int> list;

    // Test insertFront()
    for (int i = 1; i < 10; ++i) {
        list.insertFront(i);
    }
    EXPECT_EQ(getListContent(list), "987654321");

    // Test insertBack()
    list.clear();
    for (int i = 1; i < 10; ++i) {
        list.insertBack(i);
    }
    EXPECT_EQ(getListContent(list), "123456789");

    // Test insert()
    for (int i = 0; i < 8; ++i) {
        list.insert(2 * i + 1, 0);
    }
    EXPECT_EQ(getListContent(list), "10203040506070809");
    list.insert(0, 1);
    list.insert(100, 9);
    EXPECT_EQ(getListContent(list), "1102030405060708099");
}

TEST(DoublyLinkedListTest, Remove) {
    DoublyLinkedList<int> list;

    // Test removeFront()
    for (int i = 1; i < 6; ++i) {
        list.insertBack(i);
    }
    for (int i = 0; i < 3; ++i) {
        list.removeFront();
    }
    EXPECT_EQ(getListContent(list), "45");
    for (int i = 0; i < 2; ++i) {
        list.removeFront();
    }
    EXPECT_EQ(getListContent(list), "");

    // Test removeBack()
    for (int i = 1; i < 6; ++i) {
        list.insertBack(i);
    }
    for (int i = 0; i < 3; ++i) {
        list.removeBack();
    }
    EXPECT_EQ(getListContent(list), "12");
    for (int i = 0; i < 2; ++i) {
        list.removeBack();
    }
    EXPECT_EQ(getListContent(list), "");

    // Test remove()
    for (int i = 1; i < 6; ++i) {
        list.insertBack(i);
    }
    list.remove(0);
    list.remove(3);
    list.remove(100);
    EXPECT_EQ(getListContent(list), "234");
    list.remove(1);
    list.remove(1);
    EXPECT_EQ(getListContent(list), "2");
    list.remove(1);
    list.remove(0);
    EXPECT_EQ(getListContent(list), "");
}

TEST(DoublyLinkedListTest, Find) {
    DoublyLinkedList<int> list;
    for (int i = 0; i < 3; ++i) {
        list.insertBack(i);
    }
    int p = list.find(0);
    EXPECT_EQ(p, 0);
    p = list.find(1);
    EXPECT_EQ(p, 1);
    p = list.find(2);
    EXPECT_EQ(p, 2);
    p = list.find(3);
    EXPECT_EQ(p, -1);
}

TEST(DoublyLinkedListTest, Sort) {
    DoublyLinkedList<int> list;
    list.sort();
    EXPECT_EQ(getListContent(list), "");
    list.insertBack(0);
    list.sort();
    EXPECT_EQ(getListContent(list), "0");
    list.removeFront();
    list.insertFront(1);
    list.insertFront(2);
    list.sort();
    EXPECT_EQ(getListContent(list), "12");
    list.removeBack();
    list.sort();
    EXPECT_EQ(getListContent(list), "1");
    for (int i = 0; i < 5; ++i) {
        list.insertFront(i);
    }
    list.sort();
    EXPECT_EQ(getListContent(list), "011234");
    list.insertBack(2);
    list.sort();
    EXPECT_EQ(getListContent(list), "0112234");
}
