#include "catch2/catch_amalgamated.hpp"
#include "jumble/graph.hpp"
#include <string>

typedef jumble::Graph<std::string> G;
typedef G::NumType NumType;
typedef G::WeightType WeightType;

TEST_CASE("List") {
    G graph(3, G::LIST);
    REQUIRE(graph.getSize() == (NumType)3);

    // Test operator[]
    graph[0] = "AA";
    graph[1] = "BB";
    graph[2] = "CC";
    REQUIRE(graph[0] == "AA");
    REQUIRE(graph[1] == "BB");
    REQUIRE(graph[2] == "CC");

    // Test weights
    REQUIRE(G::isINF(graph.getWeight(0, 0)));
    REQUIRE(G::isINF(graph.getWeight(1, 1)));
    REQUIRE(G::isINF(graph.getWeight(2, 2)));
    REQUIRE(G::isINF(graph.getWeight(0, 1)));
    REQUIRE(G::isINF(graph.getWeight(0, 2)));
    REQUIRE(G::isINF(graph.getWeight(1, 2)));
    REQUIRE(G::isINF(graph.getWeight(1, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 1)));

    graph.setWeight(0, 1, 10);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 30);
    REQUIRE(G::isINF(graph.getWeight(0, 0)));
    REQUIRE(G::isINF(graph.getWeight(1, 1)));
    REQUIRE(G::isINF(graph.getWeight(2, 2)));
    REQUIRE(graph.getWeight(0, 1) == (WeightType)10);
    REQUIRE(graph.getWeight(0, 2) == (WeightType)20);
    REQUIRE(graph.getWeight(1, 2) == (WeightType)30);
    REQUIRE(G::isINF(graph.getWeight(1, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 1)));

    graph.setWeight(0, 1, 40);
    graph.setWeight(0, 2, 50);
    graph.setWeight(1, 2, 60);
    REQUIRE(G::isINF(graph.getWeight(0, 0)));
    REQUIRE(G::isINF(graph.getWeight(1, 1)));
    REQUIRE(G::isINF(graph.getWeight(2, 2)));
    REQUIRE(graph.getWeight(0, 1) == (WeightType)40);
    REQUIRE(graph.getWeight(0, 2) == (WeightType)50);
    REQUIRE(graph.getWeight(1, 2) == (WeightType)60);
    REQUIRE(G::isINF(graph.getWeight(1, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 1)));

    // Test neighbors
    auto n0 = graph.getNeighbors(0);
    REQUIRE(n0.size() == (decltype(n0)::size_type)2);
    REQUIRE(n0[0] == 1);
    REQUIRE(n0[1] == 2);

    auto n1 = graph.getNeighbors(1);
    REQUIRE(n1.size() == (decltype(n1)::size_type)1);
    REQUIRE(n1[0] == 2);

    auto n2 = graph.getNeighbors(2);
    REQUIRE(n2.size() == (decltype(n2)::size_type)0);
}

TEST_CASE("Matrix") {
    G graph(3, G::MATRIX);
    REQUIRE(graph.getSize() == (NumType)3);

    // Test operator[]
    graph[0] = "AA";
    graph[1] = "BB";
    graph[2] = "CC";
    REQUIRE(graph[0] == "AA");
    REQUIRE(graph[1] == "BB");
    REQUIRE(graph[2] == "CC");

    // Test weights
    REQUIRE(graph.getWeight(0, 0) == (WeightType)0);
    REQUIRE(graph.getWeight(1, 1) == (WeightType)0);
    REQUIRE(graph.getWeight(2, 2) == (WeightType)0);
    REQUIRE(G::isINF(graph.getWeight(0, 1)));
    REQUIRE(G::isINF(graph.getWeight(0, 2)));
    REQUIRE(G::isINF(graph.getWeight(1, 2)));
    REQUIRE(G::isINF(graph.getWeight(1, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 1)));

    graph.setWeight(0, 1, 10);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 30);
    REQUIRE(graph.getWeight(0, 0) == (WeightType)0);
    REQUIRE(graph.getWeight(1, 1) == (WeightType)0);
    REQUIRE(graph.getWeight(2, 2) == (WeightType)0);
    REQUIRE(graph.getWeight(0, 1) == (WeightType)10);
    REQUIRE(graph.getWeight(0, 2) == (WeightType)20);
    REQUIRE(graph.getWeight(1, 2) == (WeightType)30);
    REQUIRE(G::isINF(graph.getWeight(1, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 1)));

    graph.setWeight(0, 1, 40);
    graph.setWeight(0, 2, 50);
    graph.setWeight(1, 2, 60);
    REQUIRE(graph.getWeight(0, 0) == (WeightType)0);
    REQUIRE(graph.getWeight(1, 1) == (WeightType)0);
    REQUIRE(graph.getWeight(2, 2) == (WeightType)0);
    REQUIRE(graph.getWeight(0, 1) == (WeightType)40);
    REQUIRE(graph.getWeight(0, 2) == (WeightType)50);
    REQUIRE(graph.getWeight(1, 2) == (WeightType)60);
    REQUIRE(G::isINF(graph.getWeight(1, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 0)));
    REQUIRE(G::isINF(graph.getWeight(2, 1)));

    // Test neighbors
    auto n0 = graph.getNeighbors(0);
    REQUIRE(n0.size() == (decltype(n0)::size_type)2);
    REQUIRE(n0[0] == 1);
    REQUIRE(n0[1] == 2);

    auto n1 = graph.getNeighbors(1);
    REQUIRE(n1.size() == (decltype(n1)::size_type)1);
    REQUIRE(n1[0] == 2);

    auto n2 = graph.getNeighbors(2);
    REQUIRE(n2.size() == (decltype(n2)::size_type)0);
}
