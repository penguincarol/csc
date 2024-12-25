#include <iostream>
extern "C" {
#include "kissat.h"
}
#include "KemenySolver.h"

int main() {
    std::cout << "Starting Kemeny solver\n";


    csc::solveKemeny();
    std::cout << "all possibilities tested \n";
    return 0;
}
