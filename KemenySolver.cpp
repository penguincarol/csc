#include "KemenySolver.h"
#include "AdjacencyMatrix.h"
#include "Edge.h"
#include "ScoreVector.h"

extern "C" {
#include "kissat.h"
}
#include <vector>
#include <iostream>
#include <algorithm>

namespace csc{

    void generate_at_most_k(size_t n, size_t k, kissat* solver) {
        if(k >= n) return;
        k = k+1;

        // Initialize a bitmask where the first k bits are set to 1
        //std::vector<bool> bitmask(k, true);
        //bitmask.resize(n, false);
        std::vector<bool> bitmask(n, false);
        for(auto i=0; i < k; i++){
            bitmask[i] = true;
        }

        // Generate all combinations using next_permutation
        do {
            //std::vector<int> subset;
            for (int i = 0; i < n; ++i) {
                if (bitmask[i]) {
                    //kissat_add(solver, -(i+1));
                    //std::cout << "adding" << i+1 << "\n";
                    kissat_add(solver, -(i+1));
                }
            }
            //std::cout << "ending clause\n";
            kissat_add(solver, 0);

        } while (std::prev_permutation(bitmask.begin(), bitmask.end()));
    }




    void solveKemeny(){
        AdjacencyMatrix m{};
        //(num_vertices-1 +1)/2 should be read as rounding up (num_vertices-1)/2
        for(size_t kemeny_score_node_0 = (num_vertices-1 +1)/2; kemeny_score_node_0 <= num_vertices-1; kemeny_score_node_0++){
            //std::vector<uint8_t> kemeny_scores_dominated(kemeny_score_node_0, 0);
            //std::vector<uint8_t> kemeny_scores_dominators(num_vertices - 1 - kemeny_score_node_0, 0);

            //set edges of node 0
            for(size_t i = 1; i <= kemeny_score_node_0; i++){
                m.setEdge(0, i);
            } for(size_t i = kemeny_score_node_0+1; i < num_vertices; i++){
                m.setEdge(i, 0);
            }

            //ScoreVector scoresDominators(kemeny_score_node_0, kemeny_score_node_0);
            //ScoreVector firstInvalidScoreV(scoresDominators.end());


            size_t number_of_edge_configurations = (1 << ((num_vertices-1)*(num_vertices-2) / 2)); //2^(number_of_remaining_edges) (after fixing edges of node 0)
            for(size_t i = 0; i < number_of_edge_configurations; i++){
                if(i%100 == 0){
                    std::cout << "Configuration " << i << " of " << number_of_edge_configurations << "\n";
                }
                m.setEdgesAccordingToIndex(i);
                //std::cout << i << "\n";
                std::vector<cycle> cycles(m.findCycles());
                EdgeToLiteral e2l(cycles);
                kissat *solver = kissat_init();

                //represent each cycle by a corresponding clause
                for(auto& cycle: cycles){
                    for(size_t v=1; v < cycle.size(); v++){
                        kissat_add(solver, e2l.edge_to_literal({cycle[v-1], cycle[v]}));
                    }
                    kissat_add(solver, e2l.edge_to_literal({cycle.back(), cycle.front()}));
                    kissat_add(solver, 0);
                }

                //add "at-most-k"-clauses, ensuring that only the specified number of edges can be removed (i.e. only the specified number of variables can be set to true)
                generate_at_most_k(e2l.counter-1, number_of_removable_edges, solver);
                //std::cout << "countersample:" << e2l.counter-1 << "\n";

                int result = kissat_solve(solver);  //10: satisfiable; 20: unsatisfiable
                if(result == 20){
                    std::cout << "Unsatisfiable graph found \n";
                    std::cout << "Adjacency matrix:\n";
                    m.print();
                    std::cout << "\nCycles:\n";
                    for(auto& cycle: cycles){
                        std::cout << "(";
                        for(auto v : cycle){
                            std::cout << static_cast<uint>(v) << ", ";
                        }
                        std::cout << static_cast<uint>(cycle.front());
                        std::cout << ")";
                    }
                    std::cout << "\n";
                    std::exit(0);
                }

                kissat_release(solver);
            }
        }
    }
}