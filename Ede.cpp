#include "Edge.h"

namespace csc {
    void EdgeToLiteral::addEdge(Edge e) {
        auto [_, success] = edgeToIndex.try_emplace(e, counter);
        if(success){    //if we actually added a new edge we need to increase the literal counter
            counter += 1;
        }
    }

    int EdgeToLiteral::edge_to_literal(Edge edge) const {
        return edgeToIndex.find(edge)->second;
    }

    EdgeToLiteral::EdgeToLiteral(const vector<cycle> &cycles): edgeToIndex(), counter(1) {
        for(auto& cycle: cycles){
            for(size_t i=1; i < cycle.size(); i++){
                addEdge({cycle[i-1], cycle[i]});
            }
            addEdge({cycle.back(), cycle.front()});
        }
    }
}
