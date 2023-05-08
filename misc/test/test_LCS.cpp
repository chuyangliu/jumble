#include "gtest/gtest.h"
#include "tastylib/LCS.h"

using tastylib::LCSubstr;
using tastylib::LCSubseq;

TEST(LCSTest, subsequence) {
    EXPECT_EQ((std::size_t)0, LCSubseq("", ""));
    EXPECT_EQ((std::size_t)4, LCSubseq("0x2x3x4x5", "a12345a"));
}

TEST(LCSTest, substring) {
    EXPECT_EQ((std::size_t)0, LCSubstr("", ""));
    EXPECT_EQ((std::size_t)1, LCSubstr("0x2x3x4x5", "a12345a"));
}
