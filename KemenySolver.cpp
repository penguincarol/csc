#include "KemenySolver.h"
#include "AdjacencyMatrix.h"
#include "Edge.h"

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


    struct KemenyScoreVector{
        KemenyScoreVector(size_t num_vertices_managed, uint8_t maximum_kemeny_score): score_vector(num_vertices_managed, 0), maximum_kemeny_score(maximum_kemeny_score){};

        KemenyScoreVector& operator++(){
            size_t i = score_vector.size()-1;
            score_vector[i] += 1;
            while(score_vector[i] > maximum_kemeny_score){
                score_vector[i] = 0;
                i--;
                score_vector[i] += 1;
                if(i == 0){
                    break;
                }
            }
            return *this;
        }

        bool operator==(const KemenyScoreVector& other) const{
            return other.score_vector == score_vector;
        }

        //this is not equal to the std::iterator end(), but similar. this function returns the first KemenyScoreVector that is invalid, since it contains one kemeny-rank, that is too high
        KemenyScoreVector end(){
            KemenyScoreVector returnvalue(score_vector.size(), maximum_kemeny_score);
            returnvalue.score_vector[0] = maximum_kemeny_score+1;
            return returnvalue;
        }

        std::vector<uint8_t> score_vector;
        uint8_t maximum_kemeny_score;
    };

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

            size_t number_of_edge_configurations = (1 << ((num_vertices-1)*(num_vertices-2) / 2)); //2^(number_of_remaining_edges) (after fixing edges of node 0)
            for(size_t i = 0; i < number_of_edge_configurations; i++){
                m.setEdgesAccordingToIndex(i);
                std::cout << i << "\n";
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

                int result = kissat_solve(solver);
                if(result == 20){
                    std::cout << "Unsatisfiable graph found \n";
                    m.print();
                    std::exit(-1);
                }

                kissat_release(solver);
            }
        }
    }
}