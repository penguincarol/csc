#include "GraphToSAT.h"
#include "AdjacencyMatrix.h"
#include "Edge.h"

namespace csc::g2sat{
    void addCycleBreakConstraint(kissat* SAT_instance, const std::vector<cycle>& cycles ,const EdgeToLiteral& e2l){
        for(auto& cycle: cycles){
            for(size_t i = 1; i < cycle.size(); i++){
                kissat_add(SAT_instance, e2l.edge_to_literal({cycle[i-1], cycle[i]}));
            }
            kissat_add(SAT_instance, e2l.edge_to_literal({cycle.back(), cycle.front()}));
            kissat_add(SAT_instance, 0);    //Terminate clause
        }
    }

    void addLimitedTrueVariableConstraint(kissat* SAT_instance, const EdgeToLiteral& e2l){
        //TODO
    }

    kissat* graphToSAT(const AdjacencyMatrix& m){
        std::vector<cycle> cycles = m.findCycles();
        EdgeToLiteral e2l(cycles);
        kissat* SAT_instance = kissat_init();

        addCycleBreakConstraint(SAT_instance, cycles ,e2l);
        addLimitedTrueVariableConstraint(SAT_instance, e2l);
        return SAT_instance;
    }
}
