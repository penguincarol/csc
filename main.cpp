#include <iostream>
extern "C" {
#include "kissat.h"
}
#include "KemenySolver.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    /*kissat *solver = kissat_init();
    kissat_add(solver, 1);   // Add variable x (encoded as literal 1)
    kissat_add(solver, 2);   // Add variable y (encoded as literal 2)
    kissat_add(solver, 0);   // Terminate clause (x OR y)

    kissat_add(solver, -1);  // Add clause: NOT x
    kissat_add(solver, 0);

    int result = kissat_solve(solver);
    if (result == 10) {  // 10 = SATISFIABLE
        printf("SAT\n");
    } else if (result == 20) {  // 20 = UNSATISFIABLE
        printf("UNSAT\n");
    }
    kissat_release(solver);*/

    csc::solveKemeny();
    std::cout << "all possibilities tested \n";
    return 0;
}
