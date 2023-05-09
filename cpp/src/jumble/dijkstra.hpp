#ifndef JUMBLE_DIJKSTRA_HPP_INCLUDED
#define JUMBLE_DIJKSTRA_HPP_INCLUDED

#include "jumble/graph.hpp"

JUMBLE_NAMESPACE_BEGIN

/*
The value stored in each vertex when running Dijkstra's algorithm.

@praram Value The type of other values stored in each vertex
*/
template<typename Value>
struct DijkValue {
    bool visit;
    typename Graph<DijkValue<Value>>::WeightType dist;
    typename Graph<DijkValue<Value>>::NumType prev;
    Value val;
};

/*
Graph object used for Dijkstra's algorithm.
*/
template<typename Value>
using DijkGraph = Graph<DijkValue<Value>>;

/*
The Dijkstra's algorithm to find the shortest
paths from a starting vertex to other vertices.

After its execution, for each vertex 'i' in the graph:
1. graph[i].dist denotes the minimum distance from the starting vertex to vertex 'i'.
2. graph[i].prev denotes the previous vertex of vertex 'i' on the minimum path.
If 'i' is the starting vertex, then graph[i].prev == graph.getSize() + 1.

@param graph The graph object
@param src   The starting vertex number
*/
template<typename Value>
void dijkstra(DijkGraph<Value>& graph,
              const typename DijkGraph<Value>::NumType src) {
    using G          = DijkGraph<Value>;
    using NumType    = typename DijkGraph<Value>::NumType;
    using WeightType = typename DijkGraph<Value>::WeightType;

    // Init
    NumType size = graph.getSize();
    for (NumType i = 0; i < size; ++i) {
        graph[i].visit = false;
        graph[i].dist = (i == src ? 0 : G::MAX_WEIGHT);
        graph[i].prev = size + 1;  // No previous vertex
    }

    while (true) {
        WeightType minDist = G::MAX_WEIGHT;
        NumType cur = 0;
        for (NumType i = 0; i < size; ++i) {
            if (!graph[i].visit && graph[i].dist < minDist) {
                minDist = graph[i].dist;
                cur = i;
            }
        }
        if (minDist == G::MAX_WEIGHT) {
            break;  // All vertices are visited
        }
        graph[cur].visit = true;
        std::vector<NumType> neighbors = graph.getNeighbors(cur);
        for (const auto &adj : neighbors) {
            WeightType w = graph.getWeight(cur, adj);
            if (!graph[adj].visit && graph[cur].dist + w < graph[adj].dist) {
                graph[adj].dist = graph[cur].dist + w;
                graph[adj].prev = cur;
            }
        }
    }
}

JUMBLE_NAMESPACE_END

#endif
