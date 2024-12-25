#pragma once
#include <cstdint>
#include <unordered_map>
#include "AdjacencyMatrix.h"


using namespace std;
namespace csc {
    struct Edge {
    public:
        uint8_t from;
        uint8_t to;

        //required for hashing
        bool operator==(const Edge& other) const {
            return (from == other.from) && (to == other.to);
        }
    };
}

//required such that hashmaps work for edges
namespace std {
    template <>
    struct hash<csc::Edge> {
        size_t operator()(const csc::Edge& e) const {
            size_t h1 = hash<uint8_t>{}(e.from);
            size_t h2 = hash<uint8_t>{}(e.to);
            return h1 ^ (h2 << 1); // XOR the two hash values for better distribution
        }
    };
}

namespace csc{
class EdgeToLiteral {
public:
    EdgeToLiteral(const std::vector<cycle>& cycles);

    void addEdge(Edge e);

    int edge_to_literal(Edge edge) const;

    std::unordered_map<Edge, uint32_t> edgeToIndex;
    int counter = 1;
};
}

