#pragma once

#include "grid.h"
#include "std_array.h"
#include "functors.h"
#include "persistent_map.h"

namespace noise {

template <typename FloatT>
struct voronoi
{
    typedef std::array<FloatT, 2> vec2_t;
    typedef std::array<FloatT, 3> vec3_t;

    typedef std::array<int, 2>    int2_t;
    typedef std::array<int, 3>    int3_t;

    FloatT                              _cell_size;
    size_t                              _seed;
    persistent_map<FloatT, 2, uint64_t> _map;

    voronoi(FloatT seed, const std::array<FloatT, 2> &offset, FloatT cell_size = FloatT(256)) : _cell_size(cell_size), _seed(seed), _map(seed) {}

    FloatT evaluate(const vec2_t &p) const
    {
        vec2_t v(p / _cell_size), u(floor(v));
        FloatT max(MAX), min_dsq[2] = { max, max };
        vec2_t best_r[2] = { vec2_t { max, max }, vec2_t { max, max } };
        for (size_t i = 0; i < 9; i++) {
            auto r = _from_cell(u + DIRECTIONS[i]);
            FloatT dsq = distance_sq(v, r);
            if (dsq < min_dsq[1]) {
                best_r[1] = r;
                if ((min_dsq[1] = dsq) < min_dsq[0]) {
                    std::swap(min_dsq[0], min_dsq[1]);
                    std::swap(best_r[0], best_r[1]);
                }
            }
        }
        FloatT scale(1), d = sqrt(min_dsq[1]) - sqrt(min_dsq[0]) * scale;
        return _soft_clamp(d * (2 / scale) - 1); // scale to 1, -1 and clamp
    }

    FloatT evaluate(const vec3_t &p) const
    {
        return evaluate(vec2_t({ p[0], p[1] }));
    }

private:
    
    inline static FloatT _soft_clamp(FloatT v, FloatT hard = 9)
    {
        if (v > -1 && v < 1) return 2 / (1 + exp(-(6 + hard) * v)) - 1;
        return v < -1 ? -1 : 1;
    }
    
    constexpr static const FloatT MAX = std::numeric_limits<FloatT>::max();
    
    constexpr static const vec2_t DIRECTIONS[] = {
        vec2_t{ -1, -1 }, vec2_t{  0, -1 }, vec2_t{ 1, -1 },
        vec2_t{ -1,  0 }, vec2_t{  0,  0 }, vec2_t{ 1,  0 },
        vec2_t{ -1,  1 }, vec2_t{  0,  1 }, vec2_t{ 1,  1 }
    };

    inline const vec2_t _from_cell(const vec2_t &floor_v) const
    {
        auto r = _map(floor_v);
        return floor_v + vec2_t({ (r[0] & 0xffff) / 65535.0, (r[1] & 0xffff) / 65535.0 });
    }
};

}
