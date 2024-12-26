#include "KemenySolver.h"
#include "AdjacencyMatrix.h"
#include "Edge.h"
#include "ScoreVector.h"
#include "BitMapsForScoreVector.h"

extern "C" {
#include "kissat.h"
}
#include <vector>
#include <iostream>
#include <algorithm>

namespace csc{
    namespace log_info{
        size_t counter_trivial_instances = 0;
        size_t counter_non_trivial_instances = 0;

        void print_log_info(){
            std::cout << "number of trivial instances (no kissat needed): " << counter_non_trivial_instances << "\n"
            << "number of non-trivial instances (number of times kissat used): " << counter_trivial_instances << "\n";
        }
    }

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
            const size_t start_index_dominators = kemeny_score_node_0+1;
            const size_t start_index_dominated = 1;
            const size_t number_dominators = num_vertices - kemeny_score_node_0 - 1;
            const size_t number_dominated = kemeny_score_node_0;
            //set edges of node 0
            for(size_t i = start_index_dominated; i < start_index_dominators; i++){
                m.setEdge(0, i);
            } for(size_t i = start_index_dominators; i < num_vertices; i++){
                m.setEdge(i, 0);
            }

            //there are kemeny_score_node_0 many vertices that are being dominated by node 0. their maximum score can be kemeny_score_node_0
            ScoreVector scoresDominatedVertices(kemeny_score_node_0, std::min(kemeny_score_node_0, number_dominators));
            ScoreVector firstInvalidScoreV(scoresDominatedVertices.end());
            while(scoresDominatedVertices != firstInvalidScoreV){

                //set edges between node 1 (dominated by node 0) and the dominators of node 0
                {
                    size_t goal = std::min(num_vertices, start_index_dominators + scoresDominatedVertices.score_vector[0]);
                    for (vertex i = start_index_dominators;
                         i < goal; i++) {
                        //anchor
                        m.setEdge(1, i);
                    }
                    for (vertex i = start_index_dominators + scoresDominatedVertices.score_vector[0];
                         i < num_vertices; i++) {
                        m.setEdge(i, 1);
                    }
                }

                //the bitmaps define the edges between the dominators and the dominated (w.r.t. node 0).
                //obviously the graph is not bipartit, but these are the "bipartit edges" between those 2 sets of nodes
                BitMapsForScoreVector bipartitEdgesBitmap(scoresDominatedVertices, number_dominators);
                bool nextEdgeConfiguration = true;
                while(nextEdgeConfiguration){
                    m.setBipartitEdges(bipartitEdgesBitmap, number_dominated);
                    const size_t number_of_remaining_edges = (number_dominated*(number_dominated-1)/2) + ((number_dominators*(number_dominators-1))/2);
                    const size_t number_edge_configurations = (1 << number_of_remaining_edges); //2^(number_of_remaining_edges)
                    for(size_t i = 0; i < number_edge_configurations; i++){
                        m.setNonBipartitEdgesAccordingToIndex(i, number_dominated);
                        //TODO: tournament graph should be set here and we can create SAT-formula
                        //TODO: can still filter out if kemeny-score of individual vertices is too high, or number of cycles is low enough..
#ifdef DEBUG
                        std::cout << "next graph: \n";
                        m.print();
                        if(!m.isTournamentGraph()){
                            std::cout<<  "not a tournament graph\n";
                            exit(0);
                        }
#endif
                        std::vector<cycle> cycles(m.findCycles());
                        if(cycles.size() <= number_of_removable_edges){ //graphs, where we can remove 1 edge for each cycle are trivially solvable
                            log_info::counter_trivial_instances +=1;
                            continue;
                        } else{
                            log_info::counter_non_trivial_instances +=1;
                        }
                        EdgeToLiteral e2l(cycles);
                        kissat *solver = kissat_init();
                        for(auto& cycle: cycles){
                            for(size_t v=1; v < cycle.size(); v++){
                                kissat_add(solver, e2l.edge_to_literal({cycle[v-1], cycle[v]}));
                            }
                            kissat_add(solver, e2l.edge_to_literal({cycle.back(), cycle.front()}));
                            kissat_add(solver, 0);
                        }

                        //add "at-most-k"-clauses, ensuring that only the specified number of edges can be removed (i.e. only the specified number of variables can be set to true)
                        generate_at_most_k(e2l.counter-1, number_of_removable_edges, solver);

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

                            log_info::print_log_info();
                            std::exit(0);
                        }

                        kissat_release(solver);
                    }
                    nextEdgeConfiguration = ++bipartitEdgesBitmap;
                }
                ++scoresDominatedVertices;
            }
            /*
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
            }*/
        }
        log_info::print_log_info();
    }
}