#pragma once
#include <array>
#include <vector>
#include <cstdint>

namespace csc{
    using namespace std;
    constexpr size_t num_vertices = 7;
    constexpr size_t number_of_removable_edges = 6;
    constexpr size_t number_of_edges = num_vertices*(num_vertices-1)/2;
    //#define DEBUG 1
    //#define ONLY_COUNT_PLEASE

    //num_vertices = 7; num_removable_edges = 5: didn't work
    //num_vertices = 7; num_removable_edges = 6: that.. worked?!"

    using vertex = uint8_t;
    using cycle = std::vector<vertex>;
}