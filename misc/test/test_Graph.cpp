#include "gtest/gtest.h"
#include "tastylib/Graph.h"
#include <string>

using tastylib::Graph;

typedef Graph<std::string> G;
typedef G::NumType NumType;
typedef G::WeightType WeightType;

TEST(GraphTest, List) {
    G graph(3, G::LIST);
    EXPECT_EQ(graph.getSize(), (NumType)3);
    // Test operator[]
    graph[0] = "AA";
    graph[1] = "BB";
    graph[2] = "CC";
    EXPECT_EQ(graph[0], "AA");
    EXPECT_EQ(graph[1], "BB");
    EXPECT_EQ(graph[2], "CC");
    // Test weights
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 1)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 2)));
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 1)));
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 2)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 2)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 1)));
    graph.setWeight(0, 1, 10);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 30);
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 1)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 2)));
    EXPECT_EQ(graph.getWeight(0, 1), (WeightType)10);
    EXPECT_EQ(graph.getWeight(0, 2), (WeightType)20);
    EXPECT_EQ(graph.getWeight(1, 2), (WeightType)30);
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 1)));
    graph.setWeight(0, 1, 40);
    graph.setWeight(0, 2, 50);
    graph.setWeight(1, 2, 60);
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 1)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 2)));
    EXPECT_EQ(graph.getWeight(0, 1), (WeightType)40);
    EXPECT_EQ(graph.getWeight(0, 2), (WeightType)50);
    EXPECT_EQ(graph.getWeight(1, 2), (WeightType)60);
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 1)));
    // Test neighbors
    auto n0 = graph.getNeighbors(0);
    EXPECT_EQ(n0.size(), (decltype(n0)::size_type)2);
    EXPECT_TRUE(n0[0] == 1);
    EXPECT_TRUE(n0[1] == 2);
    auto n1 = graph.getNeighbors(1);
    EXPECT_EQ(n1.size(), (decltype(n1)::size_type)1);
    EXPECT_TRUE(n1[0] == 2);
    auto n2 = graph.getNeighbors(2);
    EXPECT_EQ(n2.size(), (decltype(n2)::size_type)0);
}

TEST(GraphTest, Matrix) {
    G graph(3, G::MATRIX);
    EXPECT_EQ(graph.getSize(), (NumType)3);
    // Test operator[]
    graph[0] = "AA";
    graph[1] = "BB";
    graph[2] = "CC";
    EXPECT_EQ(graph[0], "AA");
    EXPECT_EQ(graph[1], "BB");
    EXPECT_EQ(graph[2], "CC");
    // Test weights
    EXPECT_EQ(graph.getWeight(0, 0), (WeightType)0);
    EXPECT_EQ(graph.getWeight(1, 1), (WeightType)0);
    EXPECT_EQ(graph.getWeight(2, 2), (WeightType)0);
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 1)));
    EXPECT_TRUE(G::isINF(graph.getWeight(0, 2)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 2)));
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 1)));
    graph.setWeight(0, 1, 10);
    graph.setWeight(0, 2, 20);
    graph.setWeight(1, 2, 30);
    EXPECT_EQ(graph.getWeight(0, 0), (WeightType)0);
    EXPECT_EQ(graph.getWeight(1, 1), (WeightType)0);
    EXPECT_EQ(graph.getWeight(2, 2), (WeightType)0);
    EXPECT_EQ(graph.getWeight(0, 1), (WeightType)10);
    EXPECT_EQ(graph.getWeight(0, 2), (WeightType)20);
    EXPECT_EQ(graph.getWeight(1, 2), (WeightType)30);
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 1)));
    graph.setWeight(0, 1, 40);
    graph.setWeight(0, 2, 50);
    graph.setWeight(1, 2, 60);
    EXPECT_EQ(graph.getWeight(0, 0), (WeightType)0);
    EXPECT_EQ(graph.getWeight(1, 1), (WeightType)0);
    EXPECT_EQ(graph.getWeight(2, 2), (WeightType)0);
    EXPECT_EQ(graph.getWeight(0, 1), (WeightType)40);
    EXPECT_EQ(graph.getWeight(0, 2), (WeightType)50);
    EXPECT_EQ(graph.getWeight(1, 2), (WeightType)60);
    EXPECT_TRUE(G::isINF(graph.getWeight(1, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 0)));
    EXPECT_TRUE(G::isINF(graph.getWeight(2, 1)));
    // Test neighbors
    auto n0 = graph.getNeighbors(0);
    EXPECT_EQ(n0.size(), (decltype(n0)::size_type)2);
    EXPECT_TRUE(n0[0] == 1);
    EXPECT_TRUE(n0[1] == 2);
    auto n1 = graph.getNeighbors(1);
    EXPECT_EQ(n1.size(), (decltype(n1)::size_type)1);
    EXPECT_TRUE(n1[0] == 2);
    auto n2 = graph.getNeighbors(2);
    EXPECT_EQ(n2.size(), (decltype(n2)::size_type)0);
}
