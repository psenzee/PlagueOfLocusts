#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "grid.h"

namespace wave {

template <size_t W, size_t H, size_t N>
struct patch : public grid::fixed<std::bitset<N>, W, H>
{
    typedef patch<W, H, N>                    self_t;
    typedef std::bitset<N>                    options_t;
    typedef grid::readable<options_t>         grid_t;
    typedef grid::fixed<options_t, W, H>      base_t;

    inline patch() : base_t(options_t()) {}
    inline patch(const self_t &other) : base_t(other) {}
    inline patch(const grid_t &other) : base_t(other) {}
    
    static constexpr size_t bit_count() { return N; }

    void combine(const self_t &pt)
    {
        for (size_t i = 0; i < directions::MAX_DIRECTIONS; i++) {
            point2i p(directions::offset(base_t::CENTER, i));
            this->set(p, pt.get(p) | this->get(p));
        }
    }

    void clear(const options_t &op) { grid::clear(*this, op); }
};

}
