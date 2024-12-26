#include "AdjacencyMatrix.h"
#include "BitMapsForScoreVector.h"

#include <cassert>
#include <iostream>

namespace csc{
    void AdjacencyMatrix::setEdge(size_t from, size_t to) {
        adjacency_matr[from][to] = true;
        adjacency_matr[to][from] = false;

        /*//check that the edge, that is to be set didn't get defined (in any direction) before
        assert(!adjacency_matr[from][to]);
        assert(!adjacency_matr[to][from]);
        adjacency_matr[from][to] = true;*/
    }

    bool AdjacencyMatrix::isTournamentGraph() const {
        for(size_t i=0; i < num_vertices; i++){
            for(size_t j=i+1; j < num_vertices; j++){
                if(!AdjacencyMatrix::doVerticesHaveOneDirectedEdge(i, j)){
                    std::cout << "Vertices " << i << " and " << j << "do not have exactly one directed edge between each other \n";
                    return false;
                }
            }
        }
        for(size_t i=0; i < num_vertices; i++){
            if(adjacency_matr[i][i]){
                std::cout << "Vertex " << i << " has a reflexive edge\n";
                return false;
            }
        }
        return true;
    }

    bool AdjacencyMatrix::doVerticesHaveOneDirectedEdge(size_t vertex1, size_t vertex2) const {
        return adjacency_matr[vertex1][vertex2] xor adjacency_matr[vertex2][vertex1];
    }

    void AdjacencyMatrix::findCyclesWithStartVertex(vertex v, std::vector<vertex>& blocked, std::vector<vertex>& blockList,
                                   std::vector<vertex>& stack, std::vector<cycle>& allCycles) const {
        stack.push_back(v);
        blocked[v] = 1;

        for(vertex neighbour = 0; neighbour < num_vertices; neighbour++){
            if(adjacency_matr[v][neighbour]){
                if (neighbour == stack.front()) {  // Found a cycle
                    allCycles.push_back(stack);
                } else if (!blocked[neighbour]) {
                    findCyclesWithStartVertex(neighbour, blocked, blockList, stack, allCycles);
                }
            }
        }

        stack.pop_back();
        blocked[v] = 0;
    }

    //I still need to convince myself, that this function does in fact do as I claim
    std::vector<cycle> AdjacencyMatrix::findCycles() const{
        assert(isTournamentGraph());

        std::vector<cycle> allCycles;
        std::vector<vertex> vertex_stack;

        for (int i = 0; i < num_vertices; ++i) {
            std::vector<vertex> blocked(num_vertices, 0);
            std::vector<vertex> blockList(num_vertices);

            findCyclesWithStartVertex(i, blocked, blockList, vertex_stack, allCycles);
        }
        return allCycles;
    }

    bool AdjacencyMatrix::isForwardCycle(const cycle& c){
        assert(c.size() >= 3);
        vertex prev;
        vertex next;
        for(size_t i = 1; i < c.size(); i++){
            prev = c[i-1];
            next = c[i];
            if(!adjacency_matr[prev][next]){
                return false;
            }
        }
        if(!adjacency_matr[c.back()][c[0]]){
            return false;
        }
        return true;
    }

    bool AdjacencyMatrix::isBackwardsCycle(const cycle& c){
        assert(c.size() >= 3);
        vertex prev;
        vertex next;
        for(size_t i = c.size()-1; i >= 1; i--){
            prev = c[i];
            next = c[i-1];
            if(!adjacency_matr[prev][next]){
                return false;
            }
        }
        if(!adjacency_matr[c[0]][c.back()]){
            return false;
        }
        return true;
    }

    bool AdjacencyMatrix::isCycle(const cycle& c) {
        return isForwardCycle(c) || isBackwardsCycle(c);
    }

    std::pair<vertex, vertex> AdjacencyMatrix::edgeFromEdgeNumber(size_t edgeNumber){
        vertex from = 1;
        auto num_new_edges = num_vertices-2;
        while(edgeNumber >= num_new_edges){
            edgeNumber -= num_new_edges;
            num_new_edges -= 1;
            from += 1;
        }
        return std::pair<vertex, vertex >{from,from+edgeNumber+1};
    }

    void AdjacencyMatrix::setEdgesAccordingToIndex(size_t index) {
        size_t number_remaining_edges = (num_vertices-1)*(num_vertices-2) /2;
        for(size_t edgeNumber = 0; edgeNumber < number_remaining_edges; edgeNumber++){
            auto [v1, v2] = edgeFromEdgeNumber(edgeNumber);
            if((index >> edgeNumber) & 0b1){
                setEdge(v1, v2);
            }else{
                setEdge(v2, v1);
            }
        }
        assert(isTournamentGraph());
    }

    void AdjacencyMatrix::print() const {
        for(auto& row: adjacency_matr){
            for(auto entry: row){
                if(entry){
                    std::cout << "1 ";
                }else{
                    std::cout << "0 ";
                }
            }
            std::cout << "\n";
        }
    }

    void AdjacencyMatrix::setBipartitEdges(const BitMapsForScoreVector &bitmaps, const size_t number_dominated) {
        for(size_t i_bitmap=0; i_bitmap < bitmaps.bitmaps.size(); i_bitmap++){
            const auto& bitmap = bitmaps.bitmaps[i_bitmap];
            const vertex addressed_vertex = i_bitmap + 2; //the first bitmap (i_bitmap=0) corresponds to node 2
            for(size_t i_edge=0; i_edge < bitmap.current().size(); i_edge++){
                const vertex target = i_edge + number_dominated + 1;    //TODO: step through with debug to see if this is correct
                if(bitmap.current()[i_edge]){
                    setEdge(addressed_vertex, target);
                }else{
                    setEdge(target, addressed_vertex);
                }
            }
        }
    }

    void AdjacencyMatrix::setNonBipartitEdgesAccordingToIndex(size_t index, size_t number_dominated) {
        size_t counter = 0;
        //setEdges in dominated-subgraph
        for(vertex v1 = 1; v1 < number_dominated; v1++){
            for(vertex v2 = v1+1; v2 <= number_dominated; v2++){
                if((index>>counter)&0b1){
                    setEdge(v1, v2);
                }else{
                    setEdge(v2, v1);
                }
                counter++;
            }
        }

        //setEdges in dominator-subgraph
        for(vertex v1 = number_dominated+1; v1 < num_vertices-1; v1++){
            for(vertex v2 = v1+1; v2 < num_vertices; v2++){
                if((index>>counter)&0b1){
                    setEdge(v1, v2);
                }else{
                    setEdge(v2, v1);
                }
                counter++;
            }
        }
    }

}
