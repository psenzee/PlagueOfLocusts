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
#include "bitsets.h"
#include "grid.h"
#include "rules.h"
#include "patch.h"
#include "std_array.h"
#include "availability_list.h"

namespace wave {

template <std::size_t N>
struct state
{
    typedef std::bitset<N>              options_t;
    typedef std::array<std::size_t, N>  bit_list_t;
    typedef grid::grid<options_t>       grid_t;
    typedef grid::basic<options_t>      basic_t;
    typedef std::deque<point2i>         points_t;
    typedef rules<N>                    rules_t;
    typedef patcher<options_t, 3, 3>    patched_t;
    typedef patch<3, 3, N>              patch_t;
    
    grid_t                 &_grid;
    const rules_t          &_rules;
    options_t               _max_entropy;
    std::mt19937            _mt;
    grid::boolean           _resolve;
    std::deque<point2i>     _points;
    
    state(grid_t &grid, const rules_t &rules, const std::mt19937 &mt, const point2i &initial_point = point2i { 0, 0 })
        : _grid(grid), _rules(rules), _resolve(_grid.size()), _mt(mt) {}

    bool reduce(const point2i &p)
    {
        return _reduce(p, options_t(true));
    }
    
    bool process(const point2i &q)
    {
        if (!_process(q)) {
            return false;
        }
        point2i p;
        while ((p = find_first_set_point(_resolve)).x != -1) {
            if (!_process(p)) {
                std::cout << "!process" << std::endl;
            }
        }
        return false;
    }

private:

    bool can_apply_one(grid_t &grid, const point2i &p, const patch_t &patch)
    {
        // test if any intersections are zero
        for (size_t i = 0; i < directions::MAX_DIRECTIONS; i++) {
            auto a = directions::offset(p, i),
                 b = directions::offset(patch.CENTER, i);
            if (inside(grid, a)) {
                if ((grid.get(a) & patch.get(b)).count() == 0)
                    return false;
            }
        }
        return true;
    }
    
    void apply_one(grid_t &grid, const point2i &p, const patch_t &patch)
    {
        for (size_t i = 0; i < directions::MAX_DIRECTIONS; i++) {
            auto a = directions::offset(p, i),
                 b = directions::offset(patch.CENTER, i);
            if (inside(grid, a)) {
                auto prev = grid.get(a), value = prev & patch.get(b);
                grid.set(a, value);
                if (p == a || value.count() <= 1) {
                    _resolve.set(a, false);
                } else if (value != prev && value.count() > 1) {
                    _resolve.set(a, true);
                }
            }
        }
    }

    bool _process(const point2i &p)
    {
        //std::cout << "p " << p << std::endl;
        _resolve.set(p, false);
        options_t primary(_grid.get(p));
        if (primary == _rules.get_max_entropy()) {
            return true;
        }
        size_t count = primary.count();
        if (!count) {
            return false;
        }
        primary = _valid_options(p, primary);
        if (primary.none()) {
            std::cout << "failed" << std::endl;
            return false;
        }
        apply_one(_grid, p, _rules.patch(primary));
        return true;
    }
    
    bool _evaluate(const point2i &p, options_t options)
    {
        options_t o = _grid.get(p);
        size_t count = o.count();
        if (count <= 1) {
            return (count == 1);
        }
        options_t out = _valid_options(p, o & options);
        count = out.count();
        if (count != 0 && (count == 1 || out != (o & options) || o != (o & options))) {
            _grid.set(p, out);
            _resolve.set(p, false); // should this be here?
            return true;
        }
        return false;
    }

    bool _reduce(const point2i &p, options_t options)
    {
        if (_evaluate(p, options)) {
            return true;
        }

        // remove randomly from the least likely bits first
        options_t o(options & _grid.get(p));
        
        for (size_t tries = 16; tries > 0; ) {
            options_t r(_rules.removable_bits_from_random(_mt()));
            options_t nr = ~r & o;
            if (nr != o) {
                if (_evaluate(p, nr)) {
                    return true;
                }
                tries--;
            }
        }

        // failing that..
        // reduce the options by a different bit each time and reevaluate
        bit_list_t arr(list_from_bitset<256, std::size_t>(o));
        auto count = o.count();
        for (size_t i = 0; i < count; i++) {
            bit_list_remove_random(arr, _mt);
            auto op = bitset_from_list(arr);
            if (_evaluate(p, op)) {
                return true;
            }
        }
        return false;
    }

    options_t _valid_options(const point2i &p, const options_t &options)
    {
        // select only the bits that result in a valid state
        bit_list_t list(list_from_bitset<256, std::size_t>(options));
        options_t out;
        for (std::size_t i = 0, count = options.count(); i < count; i++) {
            options_t bits;
            bits.set(list[i]);
            auto patch = _rules.patch(bits);
            if (can_apply_one(_grid, p, patch))
                out |= bits;
        }
        return out;
    }
};

}
