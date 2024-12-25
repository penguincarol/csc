#pragma once
#include <array>

namespace csc{
    using namespace std;
    constexpr size_t num_vertices = 6;
    constexpr size_t number_of_edges = num_vertices*(num_vertices-1)/2;
    constexpr size_t number_of_removable_edges = 3;

    //num_vertices = 7; num_removable_edges = 20: worked
    //num_vertices = 7; num_removable_edges = 10:

    using vertex = uint8_t;
    using cycle = std::vector<vertex>;
}