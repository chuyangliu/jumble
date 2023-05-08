#include "gtest/gtest.h"
#include "tastylib/HashTable.h"
#include <string>

using tastylib::HashTable;

typedef HashTable<std::string>::SizeType SizeType;

TEST(HashTableTest, Basic) {
    HashTable<std::string> table;
    table.clear();
    EXPECT_TRUE(table.isEmpty());
    table.insert("Alice");
    table.insert("Alice");
    table.insert("Darth");
    EXPECT_EQ(table.getSize(), (SizeType)2);
    EXPECT_TRUE(table.has("Alice"));
    EXPECT_TRUE(table.has("Darth"));
    table.remove("Darth");
    table.remove("Bob");
    EXPECT_TRUE(table.has("Alice"));
    EXPECT_FALSE(table.has("Darth"));
    EXPECT_EQ(table.getSize(), (SizeType)1);
    table.clear();
    EXPECT_EQ(table.getSize(), (SizeType)0);
}

TEST(HashTableTest, Rehash) {
    HashTable<std::string> table(100);
    for (int i = 0; i < 100; ++i) {
        table.insert(std::to_string(i));
    }
    EXPECT_EQ(table.getSize(), (SizeType)100);

    {
        table.rehash(300);
        EXPECT_EQ(table.getSize(), (SizeType)100);
        bool res = true;
        for (int i = 0; i < 100; ++i) {
            if (!table.has(std::to_string(i))) {
                res = false;
                break;
            }
        }
        EXPECT_TRUE(res);
    }
    
    {
        table.rehash(500);
        EXPECT_EQ(table.getSize(), (SizeType)100);
        bool res = true;
        for (int i = 0; i < 100; ++i) {
            if (!table.has(std::to_string(i))) {
                res = false;
                break;
            }
        }
        EXPECT_TRUE(res);
    }
}
