#pragma once
#include <array>
#include <vector>
#include <cstdint>

namespace csc{
    using namespace std;
    constexpr size_t num_vertices = 8;  //should not be set below 4
    constexpr size_t number_of_removable_edges = 6;
    constexpr size_t number_of_edges = num_vertices*(num_vertices-1)/2;
    //#define ONLY_COUNT_PLEASE

    //quick ways to enable various debug information
    //#define DEBUG1 1
    //#define DEBUG2 1

    //num_vertices = 7; num_removable_edges = 5: didn't work
    //num_vertices = 7; num_removable_edges = 6: worked

    //num_vertices = 8; num_removable_edges = 6:

    using vertex = uint8_t;
    using cycle = std::vector<vertex>;
}