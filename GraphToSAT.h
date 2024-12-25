#pragma once
#include <unordered_map>
#include "AdjacencyMatrix.h"
extern "C" {
#include "kissat.h"
}


namespace csc::g2sat{
    kissat* graphToSAT(const AdjacencyMatrix& m);
}
