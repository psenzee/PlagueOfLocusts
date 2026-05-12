#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <random>
#include <algorithm>
#include <cstring>
#include <ctime>
#include <bitset>
#include "grid.h"

namespace wave {

constexpr int point_rand(const point2i &p, int seed = 0)
{
    return (std::minstd_rand(p.x * 65535 + p.y + seed))();
}

template <std::size_t N, template Functor = std::greater>
struct point_options_count_comparator
{
    typedef std::bitset<N>  options_t;
    typedef grid<options_t> grid_t;
    
    const grid_t &_grid;
    
    inline point_options_count_comparator(const grid_t &grid) : _grid(grid) {}
    
    inline bool operator()(const point2i &a, const point2i &b) const
    {
        if (&a == &b || a == b) {
            return false;
        }
        size_t ca = _grid.get(a).count(), cb = _grid.get(b).count();
        return ca == cb ? Functor(a, b) : Functor(ca, cb);
    }
};

}
