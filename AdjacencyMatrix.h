#pragma once
#include <array>
#include <cstdint>
#include <vector>
#include "constants.h"

using namespace std;
namespace csc{
    class AdjacencyMatrix {
    public:
        void setEdge(size_t from, size_t to);
        [[nodiscard]] std::vector<cycle> findCycles() const;

        void print() const;

        [[nodiscard]] bool isTournamentGraph() const;

        void setEdgesAccordingToIndex(size_t index);
    private:
        array<array<bool, num_vertices>, num_vertices> adjacency_matr;

        bool isForwardCycle(const cycle& c);
        bool isBackwardsCycle(const cycle& c);
        bool isCycle(const cycle& c);
        static std::pair<vertex, vertex> edgeFromEdgeNumber(size_t edgeNumber);

        void findCyclesWithStartVertex(vertex v, std::vector<vertex>& blocked, std::vector<vertex>& blockList,
                                     std::vector<vertex>& stack, std::vector<cycle>& allCycles) const;

        [[nodiscard]] inline bool doVerticesHaveOneDirectedEdge(size_t vertex1, size_t vertex2) const;
    };

}
