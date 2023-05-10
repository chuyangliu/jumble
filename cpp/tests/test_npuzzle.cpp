#include "catch2/catch_amalgamated.hpp"
#include "jumble/npuzzle.hpp"

using jumble::PuzzleNode;
using jumble::NPuzzle;
using jumble::Direc;

typedef PuzzleNode::SizeType SizeType;

TEST_CASE("Node") {
    PuzzleNode oriNode({1, 2, 3, 4, 0, 5, 6, 7, 8}, 3, 3);
    PuzzleNode node({1, 2, 3, 4, 0, 5, 6, 7, 8}, 3, 3);
    REQUIRE(node.getRowCount() == (SizeType)3);
    REQUIRE(node.getColCount() == (SizeType)3);
    REQUIRE(node.canMove(Direc::NONE));
    REQUIRE(!node.canMove(Direc(1000)));

    node.move(Direc::NONE);
    node.move(Direc(1000));
    REQUIRE(node.getDirectionTo(nullptr) == Direc::NONE);
    REQUIRE(node.toString() == "{1, 2, 3, 4, 0, 5, 6, 7, 8}");
    REQUIRE(node.canMove(Direc::LEFT));
    REQUIRE(node.canMove(Direc::UP));
    REQUIRE(node.canMove(Direc::RIGHT));
    REQUIRE(node.canMove(Direc::DOWN));

    node.move(Direc::LEFT);
    REQUIRE(node.toString() == "{1, 2, 3, 0, 4, 5, 6, 7, 8}");
    REQUIRE(!node.canMove(Direc::LEFT));
    REQUIRE(node.canMove(Direc::UP));
    REQUIRE(node.canMove(Direc::RIGHT));
    REQUIRE(node.canMove(Direc::DOWN));
    REQUIRE(node.getDirectionTo(&oriNode) == Direc::LEFT);

    node.move(Direc::RIGHT);
    node.move(Direc::RIGHT);
    REQUIRE(node.toString() == "{1, 2, 3, 4, 5, 0, 6, 7, 8}");
    REQUIRE(node.canMove(Direc::LEFT));
    REQUIRE(node.canMove(Direc::UP));
    REQUIRE(!node.canMove(Direc::RIGHT));
    REQUIRE(node.canMove(Direc::DOWN));
    REQUIRE(node.getDirectionTo(&oriNode) == Direc::RIGHT);

    node.move(Direc::LEFT);
    node.move(Direc::UP);
    REQUIRE(node.toString() == "{1, 0, 3, 4, 2, 5, 6, 7, 8}");
    REQUIRE(node.canMove(Direc::LEFT));
    REQUIRE(!node.canMove(Direc::UP));
    REQUIRE(node.canMove(Direc::RIGHT));
    REQUIRE(node.canMove(Direc::DOWN));
    REQUIRE(node.getDirectionTo(&oriNode) == Direc::UP);

    node.move(Direc::DOWN);
    node.move(Direc::DOWN);
    REQUIRE(node.toString() == "{1, 2, 3, 4, 7, 5, 6, 0, 8}");
    REQUIRE(node.canMove(Direc::LEFT));
    REQUIRE(node.canMove(Direc::UP));
    REQUIRE(node.canMove(Direc::RIGHT));
    REQUIRE(!node.canMove(Direc::DOWN));
    REQUIRE(node.getDirectionTo(&oriNode) == Direc::DOWN);

    node.move(Direc::LEFT);
    REQUIRE(node.toString() == "{1, 2, 3, 4, 7, 5, 0, 6, 8}");
    REQUIRE(!node.canMove(Direc::LEFT));
    REQUIRE(node.canMove(Direc::UP));
    REQUIRE(node.canMove(Direc::RIGHT));
    REQUIRE(!node.canMove(Direc::DOWN));
    REQUIRE(node.getDirectionTo(&oriNode) == Direc::NONE);
}

TEST_CASE("Basic") {
    PuzzleNode oriBeg({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 4, 4);
    PuzzleNode oriEnd({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15}, 4, 4);

    NPuzzle puzzle(oriBeg, oriEnd);
    auto beg = puzzle.shuffleBeg();
    auto end = puzzle.shuffleEnd();
    REQUIRE(beg == puzzle.getBeg());
    REQUIRE(end == puzzle.getEnd());

    puzzle.setBeg(oriBeg);
    puzzle.setEnd(oriEnd);
    REQUIRE(puzzle.getBeg() == puzzle.getEnd());
}

TEST_CASE("AStar") {
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
    REQUIRE(beg == end);
}
