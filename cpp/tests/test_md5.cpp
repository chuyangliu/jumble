#include "catch2/catch_amalgamated.hpp"
#include "jumble/md5.hpp"

const auto md5 = jumble::MD5::getInstance();

TEST_CASE("EmptyString") {
    REQUIRE(md5->hash("") == "d41d8cd98f00b204e9800998ecf8427e");
}

TEST_CASE("ShortString") {
    REQUIRE(md5->hash("TastyLib") == "2dabbfd553b67530e4892eb9481121fa");
    REQUIRE(md5->hash("The quick brown fox jumps over the lazy dog") == "9e107d9d372bb6826bd81d3542a419d6");
    REQUIRE(md5->hash("The quick brown fox jumps over the lazy dog.") == "e4d909c290d0fb1ca068ffaddf22cbd0");
}

TEST_CASE("LongString") {
    std::string msg;
    for (int i = 0; i < 1000; ++i) {
        msg += "abcdefghijklmnopqrstuvwxyz0123456789~!@#$%^&*()_+-={}[]|;:<>,.?/";
    }
    REQUIRE(md5->hash(msg) == "836fb4e7ebd9ec4cd03ac50f2a888ce6");
}
