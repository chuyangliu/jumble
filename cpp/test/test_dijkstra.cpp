#include "catch2/catch_amalgamated.hpp"
#include "jumble/dijkstra.hpp"
#include <string>

typedef jumble::DijkGraph<std::string> G;
typedef G::NumType NumType;
typedef G::WeightType WeightType;

TEST_CASE("Simple") {
    G graph(3, G::LIST);
    graph[0].val = "Alice";
    graph[1].val = "Darth";
    graph[2].val = "Bob";
    graph.setWeight(0, 1, 5);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 5);
    jumble::dijkstra(graph, 0);
    std::vector<WeightType> expectDist = {0, 5, 10};
    for (NumType i = 0; i < graph.getSize(); ++i) {
        REQUIRE(graph[i].dist == expectDist[i]);
    }
    std::vector<NumType> expectPrev = {4, 0, 1};
    for (NumType i = 0; i < graph.getSize(); ++i) {
        REQUIRE(graph[i].prev == expectPrev[i]);
    }
    REQUIRE(graph[0].val == "Alice");
    REQUIRE(graph[1].val == "Darth");
    REQUIRE(graph[2].val == "Bob");
}

TEST_CASE("Complex") {
    G graph(8, G::MATRIX);
    graph.setWeight(1, 0, 1);
    graph.setWeight(1, 2, 2);
    graph.setWeight(1, 4, 1);
    graph.setWeight(2, 4, 3);
    graph.setWeight(2, 5, 10);
    graph.setWeight(3, 1, 4);
    graph.setWeight(3, 6, 5);
    graph.setWeight(4, 3, 2);
    graph.setWeight(4, 5, 2);
    graph.setWeight(4, 6, 8);
    graph.setWeight(4, 7, 4);
    graph.setWeight(5, 7, 6);
    graph.setWeight(7, 6, 1);

    {
        jumble::dijkstra(graph, 1);
        std::vector<WeightType> expectDist = {1, 0, 2, 3, 1, 3, 6, 5};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            REQUIRE(graph[i].dist == expectDist[i]);
        }
        std::vector<NumType> expectPrev = {1, 9, 1, 4, 1, 4, 7, 4};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            REQUIRE(graph[i].prev == expectPrev[i]);
        }
    }

    {
        jumble::dijkstra(graph, 2);
        std::vector<WeightType> expectDist = {10, 9, 0, 5, 3, 5, 8, 7};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            REQUIRE(graph[i].dist == expectDist[i]);
        }
        std::vector<NumType> expectPrev = {1, 3, 9, 4, 2, 4, 7, 4};
        for (NumType i = 0; i < graph.getSize(); ++i) {
            REQUIRE(graph[i].prev == expectPrev[i]);
        }
    }
}
