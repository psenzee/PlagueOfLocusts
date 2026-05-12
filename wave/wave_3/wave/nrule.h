#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <bitset>
#include "std.h"
#include "grid.h"
#include "bitsets.h"
#include "histogram.h"

namespace wave {

template <std::size_t N, typename IndexType>
struct nrule
{
    typedef histogram<N>                  histogram_t;
    typedef grid::fixed<histogram_t,3, 3> data_t;
    typedef std::bitset<N>                bitset_t;
    typedef grid::fixed<bitset_t, 3, 3>   patch_t;
    
    constexpr nrule(IndexType key = 0): _key(key), _count(0), _finalized(false) {}
    
    inline void           set_key(IndexType key)       { _key = key; _finalized = false; }
    inline IndexType      get_key()              const { return _key; }
    inline std::size_t    get_count()            const { _finalize(); return _count; }
    inline const patch_t &patch()                const { _finalize(); return _patch; }
    
    template <typename T> bool sample(const grid::readable<T> &grid, const point2i &at)
    {
        if (grid.get(at) != _key) {
            return false;
        }
        auto sz = _data.size();
        for (point2i p(0, 0), q(at - point2i(1, 1)); p.y < sz.h; ++p.y) {
            for (p.x = 0; p.x < sz.w; ++p.x) {
                _data.get_ref(p).increment(grid.get(p + q));
            }
        }
        _finalized = true;
        return true;
    }

    std::size_t applicable_count(const grid::readable<bitset_t> &grid, const point2i &at) const
    {
        std::size_t min_count = N;
        for (point2i p(0, 0), q(at - point2i(1, 1)); p.y <= 2; ++p.y) {
            for (p.x = 0; p.x <= 2; ++p.x) {
                if (p.x != 1 || p.y != 1) {
                    bitset_t result = _data.get(p).bits() & grid.get(p + q);
                    if (result.none()) {
                        return 0;
                    }
                    min_count = std::min(result.count(), min_count);
                }
            }
        }
        return min_count;
    }

    void clear_under(double threshold_ratio, std::size_t threshold_count)
    {
        for (int y = 0; y <= 2; ++y) {
            for (int x = 0; x <= 2; ++x) {
                auto &h = _data.get_ref(point2i(x, y));
                h.clear_under(threshold_ratio, threshold_count);
            }
        }
        _finalized = false;
    }
    
    std::ostream &print(std::ostream &os) const
    {
        os << "\n-------------------------------\n"
           << "key: " << std::fixed << std::setw(3) << _key << "\n";
        constexpr std::array<const char *, 9> DIRS({
            "NW ", " N ", "NE ",
            " W ", " . ", " E ",
            "SW ", " S ", "SE "
        });
        for (int y = 0; y <= 2; ++y) {
            for (int x = 0; x <= 2; ++x) {
                auto h = _data.get(point2i(x, y));
                if (h.total() && (y != 1 || x != 1)) {
                    os << DIRS[y * 3 + x] << " : ";
                    h.print(os) << "\n";
                }
            }
        }
        return os << "\n";
    }

private:
    
    void _finalize() const
    {
        if (!_finalized) {
            _patch = _make_patch();
            _count = _make_count();
            _finalized = true;
        }
    }
    
    patch_t _make_patch() const
    {
        patch_t p;
        for (int y = 0; y <= 2; ++y) {
            for (int x = 0; x <= 2; ++x) {
                point2i at(x, y);
                p.set(at, _data.get(at).bits());
            }
        }
        return p;
    }
    
    std::size_t _make_count() const
    {
        std::size_t total = 0;
        for (int y = -1; y < 2; ++y) {
            for (int x = -1; x < 2; ++x) {
                auto h = _data.get(point2i(x, y));
                total += h.total();
            }
        }
        return total;
    }

    IndexType             _key;
    data_t                _data;
    
    mutable   patch_t     _patch;
    mutable   std::size_t _count;
    mutable   bool        _finalized;
};

}
