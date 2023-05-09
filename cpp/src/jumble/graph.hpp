#ifndef JUMBLE_GRAPH_HPP_INCLUDED
#define JUMBLE_GRAPH_HPP_INCLUDED

#include "jumble/util/common.hpp"
#include <vector>
#include <list>
#include <cstddef>
#include <cstdint>

JUMBLE_NAMESPACE_BEGIN

/*
A data structure to implement the directed/undirected graph
concepts from mathematics. It stores a graph in an adjacency
list or matrix.

@param Value The type of the values stored in each vertex
*/
template<typename Value = int>
class Graph {
public:
    using NumType = std::size_t;
    using WeightType = std::int32_t;

    static const WeightType MAX_WEIGHT = INT32_MAX;
    static const WeightType MIN_WEIGHT = INT32_MIN;

    // Type of storage: adjacency list or matrix
    enum StorageType {
        LIST,
        MATRIX
    };

    /*
    Initialize the graph.

    @param n_    The amount of vertices in the graph
    @param type_ The storage type of the graph
    */
    explicit Graph(const NumType n_, const StorageType type_ = LIST) : type(type_) {
        vertices.resize(n_);
        if (type == MATRIX) {
            for (NumType i = 0; i < n_; ++i) {
                for (NumType j = 0; j < n_; ++j) {
                    vertices[i].weight.push_back(i == j ? 0 : MAX_WEIGHT);
                }
            }
        }
    }

    // Return the amount of vertices in the graph
    NumType getSize() const noexcept {
        return vertices.size();
    }
    
    // Return true if the weight of an edge is infinity
    static bool isINF(const WeightType w) noexcept {
        return w == MAX_WEIGHT || w == MIN_WEIGHT;
    }

    // Return the value of a given vertex
    Value& operator[](const NumType v) {
        return vertices[v].val;
    }

    /*
    Return the weight of an edge.

    @param from The starting vertex number of the edge
    @param to   The ending vertex number of the edge
    */
    WeightType getWeight(const NumType from, const NumType to) const {
        switch (type) {
            case LIST:
                for (const auto &v : vertices[from].neighbors) {
                    if (v.num == to) {
                        return v.weight;
                    }
                }
                return MAX_WEIGHT;
            case MATRIX:
            default:
                return vertices[from].weight[to];
                break;
        }
    }

    /*
    Modify the weight of an edge.

    @param from The starting vertex number of the edge
    @param to   The ending vertex number of the edge
    @param w    The weight of the edge
    */
    void setWeight(const NumType from, const NumType to, const WeightType w) {
        switch (type) {
            case LIST: {
                bool exist = false;
                auto &neighbors = vertices[from].neighbors;
                for (auto &v : neighbors) {
                    if (v.num == to) {
                        v.weight = w;
                        exist = true;
                        break;
                    }
                }
                if (!exist) {
                    neighbors.push_back(AdjVertex(to, w));
                }
                break;
            }
            case MATRIX:
            default:
                vertices[from].weight[to] = w;
                break;
        }
    }

    /*
    Return all neighbors of a given vertex.
    An edge that has the maximum or minimum weight is considered not exist.
    */
    std::vector<NumType> getNeighbors(const NumType v) const {
        std::vector<NumType> res;
        switch (type) {
            case LIST:
                for (const auto &n : vertices[v].neighbors) {
                    if (!isINF(n.weight)) {
                        res.push_back(n.num);
                    }
                }
                break;
            case MATRIX:
            default:
                for (NumType i = 0; i < vertices.size(); ++i) {
                    if (v != i && !isINF(vertices[v].weight[i])) {
                        res.push_back(i);
                    }
                }
                break;
        }
        return res;
    }

private:
    struct AdjVertex {
        NumType num;
        WeightType weight;

        AdjVertex(const NumType n, const WeightType w)
            : num(n), weight(w) {}
    };

    struct Vertex {
        Value val;
        std::list<AdjVertex> neighbors;  // Adjacency list
        std::vector<WeightType> weight;  // Adjacency matrix
    };

    StorageType type;

    std::vector<Vertex> vertices;
};

JUMBLE_NAMESPACE_END

#endif
