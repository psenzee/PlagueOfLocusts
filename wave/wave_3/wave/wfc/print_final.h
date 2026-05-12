#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include "grid.h"
#include "std_array.h"
#include "bitsets.h"

namespace wave {

template <std::size_t N>
void print_final(const grid::grid< std::bitset<N> > &grid, std::ostream &os, bool bracket)
{
    point2i p(0, 0);
    os << "[" << std::endl << std::endl;
    size2u sz(grid.size());
    int pad = bracket ? 5 : 3;
    for (p.y = 0; p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            auto o = grid.get(p);
            auto count = o.count();
            if (count == 1) {
                auto value = bitset_first(o);
                os << std::fixed << std::setw(pad) <<
                std::setfill(' ') << value << ",";
            } else {
                if (bracket) {
                    os << std::fixed << "[" << std::setw(pad + 2) <<
                    std::setfill(' ') << count << "],";
                } else {
                    os << std::fixed << std::setw(pad) <<
                    std::setfill(' ') << 0 << ",";
                }
            }
        }
        os << std::endl;
    }
    os << std::endl << "]";
}

}
