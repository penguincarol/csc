#include <iostream>
extern "C" {
#include "kissat.h"
}
#include "KemenySolver.h"
#include "constants.h"

int main() {
    std::cout << "Starting Kemeny solver with " <<  csc::num_vertices << " vertices and " << csc::number_of_removable_edges << " removable edges\n";

    csc::solveKemeny();
    std::cout << "all possibilities tested \n";
    return 0;
}
