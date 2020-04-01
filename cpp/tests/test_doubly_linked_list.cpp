#include "catch2/catch_amalgamated.hpp"
#include "jumble/doubly_linked_list.hpp"
#include <string>
#include <utility>

typedef jumble::DoublyLinkedList<int>::SizeType SizeType;

template<typename T>
std::string getListContent(const jumble::DoublyLinkedList<T>& list) {
    std::string str;
    list.traverse([&](const SizeType pos, const T& val) {
        UNUSED(pos);
        str += std::to_string(val);
    });
    return str;
}

TEST_CASE("Basic") {
    jumble::DoublyLinkedList<int> list;

    list.removeFront();
    list.removeBack();
    REQUIRE(list.isEmpty());

    list.insertBack(1);
    REQUIRE(getListContent(list) == "1");

    list.insertFront(2);
    REQUIRE(getListContent(list) == "21");

    list.insert(1, 3);
    REQUIRE(getListContent(list) == "231");

    list.insert(3, 4);
    REQUIRE(getListContent(list) == "2314");

    list.sort();
    REQUIRE(getListContent(list) == "1234");

    int p1 = list.find(3);
    REQUIRE(p1 == 2);

    list.remove(p1);
    REQUIRE(getListContent(list) == "124");

    list.removeFront();
    REQUIRE(getListContent(list) == "24");

    list.removeBack();
    REQUIRE(getListContent(list) == "2");

    int p2 = list.find(3);
    REQUIRE(p2 == -1);
    REQUIRE(list.getSize() == (SizeType)1);

    list.clear();
    REQUIRE(list.isEmpty());
    REQUIRE(list.getSize() == (SizeType)0);
}

TEST_CASE("Move") {
    jumble::DoublyLinkedList<int> list1;
    list1.insertBack(1);
    list1.insertBack(2);

    jumble::DoublyLinkedList<int> list2(std::move(list1));
    REQUIRE(list1.isEmpty());
    REQUIRE(getListContent(list2) == "12");

    list1 = std::move(list2);
    REQUIRE(list2.isEmpty());
    REQUIRE(getListContent(list1) == "12");
}

TEST_CASE("Insert") {
    jumble::DoublyLinkedList<int> list;

    for (int i = 1; i < 10; ++i) {
        list.insertFront(i);
    }
    REQUIRE(getListContent(list) == "987654321");

    list.clear();
    for (int i = 1; i < 10; ++i) {
        list.insertBack(i);
    }
    REQUIRE(getListContent(list) == "123456789");

    for (int i = 0; i < 8; ++i) {
        list.insert(2 * i + 1, 0);
    }
    REQUIRE(getListContent(list) == "10203040506070809");

    list.insert(0, 1);
    list.insert(100, 9);
    REQUIRE(getListContent(list) == "1102030405060708099");
}

TEST_CASE("Remove") {
    jumble::DoublyLinkedList<int> list;

    for (int i = 1; i < 6; ++i) {
        list.insertBack(i);
    }
    for (int i = 0; i < 3; ++i) {
        list.removeFront();
    }
    REQUIRE(getListContent(list) == "45");

    for (int i = 0; i < 2; ++i) {
        list.removeFront();
    }
    REQUIRE(getListContent(list) == "");

    for (int i = 1; i < 6; ++i) {
        list.insertBack(i);
    }
    for (int i = 0; i < 3; ++i) {
        list.removeBack();
    }
    REQUIRE(getListContent(list) == "12");

    for (int i = 0; i < 2; ++i) {
        list.removeBack();
    }
    REQUIRE(getListContent(list) == "");

    for (int i = 1; i < 6; ++i) {
        list.insertBack(i);
    }
    list.remove(0);
    list.remove(3);
    list.remove(100);
    REQUIRE(getListContent(list) == "234");

    list.remove(1);
    list.remove(1);
    REQUIRE(getListContent(list) == "2");

    list.remove(1);
    list.remove(0);
    REQUIRE(getListContent(list) == "");
}

TEST_CASE("Find") {
    jumble::DoublyLinkedList<int> list;
    for (int i = 0; i < 3; ++i) {
        list.insertBack(i);
    }

    int p = list.find(0);
    REQUIRE(p == 0);

    p = list.find(1);
    REQUIRE(p == 1);

    p = list.find(2);
    REQUIRE(p == 2);

    p = list.find(3);
    REQUIRE(p == -1);
}

TEST_CASE("Sort") {
    jumble::DoublyLinkedList<int> list;

    list.sort();
    REQUIRE(getListContent(list) == "");

    list.insertBack(0);
    list.sort();
    REQUIRE(getListContent(list) == "0");

    list.removeFront();
    list.insertFront(1);
    list.insertFront(2);
    list.sort();
    REQUIRE(getListContent(list) == "12");

    list.removeBack();
    list.sort();
    REQUIRE(getListContent(list) == "1");

    for (int i = 0; i < 5; ++i) {
        list.insertFront(i);
    }
    list.sort();
    REQUIRE(getListContent(list) == "011234");

    list.insertBack(2);
    list.sort();
    REQUIRE(getListContent(list) == "0112234");
}
