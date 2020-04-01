#include "catch2/catch_amalgamated.hpp"
#include "jumble/hash_table.hpp"
#include <string>

typedef jumble::HashTable<std::string>::SizeType SizeType;

TEST_CASE("Basic") {
    jumble::HashTable<std::string> table;
    table.clear();
    REQUIRE(table.isEmpty());

    table.insert("Alice");
    table.insert("Alice");
    table.insert("Darth");
    REQUIRE(table.getSize() == (SizeType)2);
    REQUIRE(table.has("Alice"));
    REQUIRE(table.has("Darth"));

    table.remove("Darth");
    table.remove("Bob");
    REQUIRE(table.has("Alice"));
    REQUIRE(!table.has("Darth"));
    REQUIRE(table.getSize() == (SizeType)1);

    table.clear();
    REQUIRE(table.getSize() == (SizeType)0);
}

TEST_CASE("Rehash") {
    jumble::HashTable<std::string> table(100);
    for (int i = 0; i < 100; ++i) {
        table.insert(std::to_string(i));
    }
    REQUIRE(table.getSize() == (SizeType)100);

    {
        table.rehash(300);
        REQUIRE(table.getSize() == (SizeType)100);
        bool res = true;
        for (int i = 0; i < 100; ++i) {
            if (!table.has(std::to_string(i))) {
                res = false;
                break;
            }
        }
        REQUIRE(res);
    }
    
    {
        table.rehash(500);
        REQUIRE(table.getSize() == (SizeType)100);
        bool res = true;
        for (int i = 0; i < 100; ++i) {
            if (!table.has(std::to_string(i))) {
                res = false;
                break;
            }
        }
        REQUIRE(res);
    }
}
