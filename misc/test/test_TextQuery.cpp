#include "gtest/gtest.h"
#include "tastylib/TextQuery.h"
#include <sstream>

using tastylib::Query;
using tastylib::TextQuery;


class TextQueryTest: public ::testing::Test {
protected:

    static void SetUpTestCase() {

        const std::string article =
            "Alice Emma has long flowing red hair\n"
            "Her Daddy says when the wind blows\n"
            "like a fiery bird in flight.\n"
            "through her hair, it looks almost alive,\n"
            "A beautiful fiery bird, he tells her,\n"
            "magical but untamed.\n"
            "\"Daddy, shush, there is no such thing,\"\n"
            "she tells him, at the same time wanting\n"
            "him to tell her more.\n"
            "Shyly, she asks, \"I mean, Daddy, is there?\"\n";

        std::istringstream iss(article);
        tq = new TextQuery(iss);

        iss.str(article);
        iss.clear();
        std::string line;
        lines = new std::vector<std::string>;
        while (std::getline(iss, line)) {
            lines->push_back(line);
        }
    }

    static void TearDownTestCase() {
        delete tq; tq = nullptr;
        delete lines; lines = nullptr;
    }

protected:
    static TextQuery *tq;
    static std::vector<std::string> *lines;
};

TextQuery *TextQueryTest::tq = nullptr;
std::vector<std::string> *TextQueryTest::lines = nullptr;


TEST_F(TextQueryTest, WordQuery) {
    std::ostringstream oss;

    std::string word = "apple";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 0\n");

    word = "Alice";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 1\n" +
              "(line 1) " + lines->at(0) + "\n");

    word = "Daddy";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 3\n" +
              "(line 2) " + lines->at(1) + "\n" +
              "(line 7) " + lines->at(6) + "\n" +
              "(line 10) " + lines->at(9) + "\n");

    word = "hair";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 2\n" +
              "(line 1) " + lines->at(0) + "\n" +
              "(line 4) " + lines->at(3) + "\n");

    word = "fiery";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 2\n" +
              "(line 3) " + lines->at(2) + "\n" +
              "(line 5) " + lines->at(4) + "\n");

    word = "bird";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 2\n" +
              "(line 3) " + lines->at(2) + "\n" +
              "(line 5) " + lines->at(4) + "\n");

    word = "wind";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 1\n" +
              "(line 2) " + lines->at(1) + "\n");

    word = "her";
    oss.str("");
    oss << Query(word).eval(*tq);
    EXPECT_EQ(oss.str(),
              word + " occurs: 3\n" +
              "(line 4) " + lines->at(3) + "\n" +
              "(line 5) " + lines->at(4) + "\n" +
              "(line 9) " + lines->at(8) + "\n");
}

TEST_F(TextQueryTest, NotQuery) {
    std::ostringstream oss;

    oss.str("");
    oss << (~Query("Alice")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "~(Alice) occurs: 9\n" +
              "(line 2) " + lines->at(1) + "\n" +
              "(line 3) " + lines->at(2) + "\n" +
              "(line 4) " + lines->at(3) + "\n" +
              "(line 5) " + lines->at(4) + "\n" +
              "(line 6) " + lines->at(5) + "\n" +
              "(line 7) " + lines->at(6) + "\n" +
              "(line 8) " + lines->at(7) + "\n" +
              "(line 9) " + lines->at(8) + "\n" +
              "(line 10) " + lines->at(9) + "\n");
}

TEST_F(TextQueryTest, OrQuery) {
    std::ostringstream oss;

    oss.str("");
    oss << (Query("hair") | Query("Alice")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "(hair | Alice) occurs: 2\n" +
              "(line 1) " + lines->at(0) + "\n" +
              "(line 4) " + lines->at(3) + "\n");

    oss.str("");
    oss << (Query("apple") | Query("peach")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "(apple | peach) occurs: 0\n");
}

TEST_F(TextQueryTest, AndQuery) {
    std::ostringstream oss;

    oss.str("");
    oss << (Query("hair") & Query("Alice")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "(hair & Alice) occurs: 1\n" +
              "(line 1) " + lines->at(0) + "\n");

    oss.str("");
    oss << (Query("apple") & Query("peach")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "(apple & peach) occurs: 0\n");
}

TEST_F(TextQueryTest, CompoundQuery) {
    std::ostringstream oss;

    oss.str("");
    oss << ((Query("fiery") & Query("bird")) | Query("wind")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "((fiery & bird) | wind) occurs: 3\n" +
              "(line 2) " + lines->at(1) + "\n" +
              "(line 3) " + lines->at(2) + "\n" +
              "(line 5) " + lines->at(4) + "\n");

    oss.str("");
    oss << (Query("but") | (Query("there") & ~Query("thing"))).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "(but | (there & ~(thing))) occurs: 2\n" +
              "(line 6) " + lines->at(5) + "\n" +
              "(line 10) " + lines->at(9) + "\n");

    oss.str("");
    oss << ((Query("apple") & Query("peach")) | Query("pair")).eval(*tq);
    EXPECT_EQ(oss.str(), std::string() +
              "((apple & peach) | pair) occurs: 0\n");
}
