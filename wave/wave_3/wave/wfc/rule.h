#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <algorithm>
#include <cstring>
#include <bitset>
#include "std.h"
#include "grid.h"
#include "patch.h"
#include "identifier_table.h"

namespace wave {

template <std::size_t N>
struct rule
{
    typedef patch<3, 3, N>                 patch_t;
    typedef std::bitset<N>                 options_t;
    typedef grid::readable<options_t>      readable_t;
    typedef identifier_table<N>            table_t;
    
    std::size_t _identifier;
    patch_t     _patch;
    options_t   _max_entropy;
    double      _weight;
    
    inline rule() : _identifier(0), _max_entropy(0), _weight(0.0) { options_t o; _patch.clear(o); }
    
    inline rule(const rule &r) : _identifier(r._identifier), _patch(r._patch) {}
    
    inline const options_t &get_max_entropy() const { return _max_entropy; }
    
    inline void add_weight(double weight) { _weight += weight; }
    
    std::ostream &print(std::ostream &os) const
    {
        os << "#" << _identifier << ": ";
        for (size_t i = 0; i < directions::MAX_DIRECTIONS; i++) {
            os << _patch.get(directions::offset(_patch.CENTER, i)) << " ";
        }
        return os << "weight: " << _weight << std::endl;
    }

    void add_example(const table_t &lookup, const uint8_t *map, size_t stride, const point2i &p)
    {
        _identifier = map[p.y * stride + p.x];
        or_patch(lookup, _patch, map, stride, p);
    }

    void set_diagonals(const options_t &options)
    {
        size_t indices[] = { directions::NORTHWEST, directions::NORTHEAST, directions::SOUTHEAST, directions::SOUTHWEST };
        for (size_t i = 0; i < 4; i++) {
            _patch.set(directions::offset(_patch.CENTER, indices[i]), options);
        }
    }

private:

    void or_patch(const table_t &lookup, patch_t &patch, const uint8_t *map, size_t stride, const point2i &p)
    {
        point2i r(p - patch_t::CENTER);
        size_t at = 0;
        //auto center = lookup.get(map[r.y * stride + r.x]);
        for (point2i q(0, 0); q.y < patch.size().h; q.y++) {
            for (q.x = 0; q.x < patch.size().w; q.x++) {
                uint8_t id = map[(q.y + r.y) * stride + (q.x + r.x)];
                auto options = lookup.get(id);
                _max_entropy |= options;
                _patch._data[at++] |= options;
            }
        }
    }

};

}
