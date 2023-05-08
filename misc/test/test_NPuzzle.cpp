#include "gtest/gtest.h"
#include "tastylib/NPuzzle.h"

using tastylib::PuzzleNode;
using tastylib::NPuzzle;
using tastylib::Direc;

typedef PuzzleNode::SizeType SizeType;

TEST(PuzzleNodeTest, Basic) {
    PuzzleNode oriNode({1, 2, 3, 4, 0, 5, 6, 7, 8}, 3, 3);
    PuzzleNode node({1, 2, 3, 4, 0, 5, 6, 7, 8}, 3, 3);
    EXPECT_EQ(node.getRowCount(), (SizeType)3);
    EXPECT_EQ(node.getColCount(), (SizeType)3);
    EXPECT_TRUE(node.canMove(Direc::NONE));
    EXPECT_FALSE(node.canMove(Direc(1000)));
    node.move(Direc::NONE);
    node.move(Direc(1000));
    EXPECT_EQ(node.getDirectionTo(nullptr), Direc::NONE);
    EXPECT_STREQ(node.toString().c_str(), "{1, 2, 3, 4, 0, 5, 6, 7, 8}");
    EXPECT_TRUE(node.canMove(Direc::LEFT));
    EXPECT_TRUE(node.canMove(Direc::UP));
    EXPECT_TRUE(node.canMove(Direc::RIGHT));
    EXPECT_TRUE(node.canMove(Direc::DOWN));
    node.move(Direc::LEFT);
    EXPECT_STREQ(node.toString().c_str(), "{1, 2, 3, 0, 4, 5, 6, 7, 8}");
    EXPECT_FALSE(node.canMove(Direc::LEFT));
    EXPECT_TRUE(node.canMove(Direc::UP));
    EXPECT_TRUE(node.canMove(Direc::RIGHT));
    EXPECT_TRUE(node.canMove(Direc::DOWN));
    EXPECT_EQ(node.getDirectionTo(&oriNode), Direc::LEFT);
    node.move(Direc::RIGHT);
    node.move(Direc::RIGHT);
    EXPECT_STREQ(node.toString().c_str(), "{1, 2, 3, 4, 5, 0, 6, 7, 8}");
    EXPECT_TRUE(node.canMove(Direc::LEFT));
    EXPECT_TRUE(node.canMove(Direc::UP));
    EXPECT_FALSE(node.canMove(Direc::RIGHT));
    EXPECT_TRUE(node.canMove(Direc::DOWN));
    EXPECT_EQ(node.getDirectionTo(&oriNode), Direc::RIGHT);
    node.move(Direc::LEFT);
    node.move(Direc::UP);
    EXPECT_STREQ(node.toString().c_str(), "{1, 0, 3, 4, 2, 5, 6, 7, 8}");
    EXPECT_TRUE(node.canMove(Direc::LEFT));
    EXPECT_FALSE(node.canMove(Direc::UP));
    EXPECT_TRUE(node.canMove(Direc::RIGHT));
    EXPECT_TRUE(node.canMove(Direc::DOWN));
    EXPECT_EQ(node.getDirectionTo(&oriNode), Direc::UP);
    node.move(Direc::DOWN);
    node.move(Direc::DOWN);
    EXPECT_STREQ(node.toString().c_str(), "{1, 2, 3, 4, 7, 5, 6, 0, 8}");
    EXPECT_TRUE(node.canMove(Direc::LEFT));
    EXPECT_TRUE(node.canMove(Direc::UP));
    EXPECT_TRUE(node.canMove(Direc::RIGHT));
    EXPECT_FALSE(node.canMove(Direc::DOWN));
    EXPECT_EQ(node.getDirectionTo(&oriNode), Direc::DOWN);
    node.move(Direc::LEFT);
    EXPECT_STREQ(node.toString().c_str(), "{1, 2, 3, 4, 7, 5, 0, 6, 8}");
    EXPECT_FALSE(node.canMove(Direc::LEFT));
    EXPECT_TRUE(node.canMove(Direc::UP));
    EXPECT_TRUE(node.canMove(Direc::RIGHT));
    EXPECT_FALSE(node.canMove(Direc::DOWN));
    EXPECT_EQ(node.getDirectionTo(&oriNode), Direc::NONE);
}

TEST(NPuzzleTest, Basic) {
    PuzzleNode oriBeg({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 4, 4);
    PuzzleNode oriEnd({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 4, 4);
    NPuzzle puzzle(oriBeg, oriEnd);
    auto beg = puzzle.shuffleBeg();
    auto end = puzzle.shuffleEnd();
    EXPECT_TRUE(beg == puzzle.getBeg());
    EXPECT_TRUE(end == puzzle.getEnd());
    puzzle.setBeg(oriBeg);
    puzzle.setEnd(oriEnd);
    EXPECT_TRUE(puzzle.getBeg() == puzzle.getEnd());
}

TEST(NPuzzleTest, AStar) {
    PuzzleNode beg({0, 1, 2, 3, 4, 5, 6, 7, 8}, 3, 3);
    PuzzleNode end({0, 1, 2, 3, 4, 5, 6, 7, 8}, 3, 3);
    beg.shuffle();
    end.shuffle();
    NPuzzle puzzle(beg, end);
    puzzle.solve();
    const auto &path = puzzle.getPath();
    for (const auto &d : path) {
        beg.move(d);
    }
    EXPECT_TRUE(beg == end);
}
