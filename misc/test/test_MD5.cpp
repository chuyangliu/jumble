#include "gtest/gtest.h"
#include "tastylib/MD5.h"

using tastylib::MD5;

TEST(MD5Test, EmptyString) {
    EXPECT_EQ(MD5::getInstance()->hash(""),
              "d41d8cd98f00b204e9800998ecf8427e");
}

TEST(MD5Test, ShortString) {
    EXPECT_EQ(MD5::getInstance()->hash("TastyLib"),
              "2dabbfd553b67530e4892eb9481121fa");
    EXPECT_EQ(MD5::getInstance()->hash("The quick brown fox jumps over the lazy dog"),
              "9e107d9d372bb6826bd81d3542a419d6");
    EXPECT_EQ(MD5::getInstance()->hash("The quick brown fox jumps over the lazy dog."),
              "e4d909c290d0fb1ca068ffaddf22cbd0");
}

TEST(MD5Test, LongString) {
    std::string msg;
    for (int i = 0; i < 1000; ++i) {
        msg += "abcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+-={}[]|;:<>,.?/";
    }
    EXPECT_EQ(MD5::getInstance()->hash(msg),
              "836fb4e7ebd9ec4cd03ac50f2a888ce6");
}
