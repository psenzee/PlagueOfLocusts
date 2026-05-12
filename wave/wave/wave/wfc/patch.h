#pragma once

#include <iostream>
#include <iomanip>
#include <vector>

#include "bits.h"
#include "bit_array.h"
#include "bit_vector.h"
#include "grid.h"
#include "fixed_grid.h"

namespace wave {

typedef bit_array<256> options_t;

template <size_t W, size_t H>
struct patch : public fixed_grid<options_t, W, H>
{
    typedef patch<W, H>                 self_t;
    typedef fixed_grid<options_t, W, H> base_t;
    
    inline patch() : base_t(options_t()) {}
    inline patch(const self_t &other) : base_t(other) {}
    inline patch(const grid<options_t> &other) : base_t(other) {}

    void combine(const patch &pt)
    {
        for (size_t i = 0; i < point2i::MAX_DIRECTIONS; i++) {
            point2i p(base_t::CENTER.direction_offset(i));
            this->set(p, pt.get(p) | this->get(p));
        }
    }

    void set_from(const uint8_t *map, size_t stride, const point2i &p)
    {
        point2i r(p - self_t::CENTER);
        size_t at = 0;
        for (point2i q(0, 0); q.y < H; q.y++) {
            for (q.x = 0; q.x < W; q.x++) {
                base_t::_data[at++].set_only(map[(q.y + r.y) * stride + (q.x + r.x)]);
            }
        }
    }

    void or_from(const uint8_t *map, size_t stride, const point2i &p)
    {
        point2i r(p - self_t::CENTER);
        size_t at = 0;
        for (point2i q(0, 0); q.y < H; q.y++) {
            for (q.x = 0; q.x < W; q.x++) {
                base_t::_data[at++].set(map[(q.y + r.y) * stride + (q.x + r.x)]);
            }
        }
    }

    void clear(const options_t &op) { wave::clear(*this, op); }
};

template <typename T, size_t W, size_t H>
struct patcher : public grid<T>
{
    typedef patcher<T, W, H>     self_t;
    typedef grid<T>              grid_t;
    typedef fixed_grid<T, W, H>  patch_t;

    grid_t  &_grid;
    patch_t  _patch;
    point2i  _at;
    bool     _isset;
    T        _null;

    inline patcher(grid_t &grid, const point2i &p) : _grid(grid), _isset(false) { patch(p); }
    inline patcher(grid_t &grid, T null_t) : _grid(grid), _isset(false), _null(null_t) {}
    inline patcher(const self_t &other) : _grid(other._grid), _isset(other._isset), _null(other._null), _at(other._at), _patch(other._patch) {}
    
    inline self_t &operator=(const self_t &other)         { if (this == &other) return *this; _grid = other._grid; _isset = other._isset; _null = other._null; _at = other._at; _patch = other._patch; return *this; }
    inline bool    operator==(const grid<T> &other) const { if (this == &other) return true; _isset = false; return equal(*this, other); }

    void patch(const point2i &p)
    {
        _at = p - patch_t::CENTER;
        _isset = true;
        for (point2i p(0, 0); p.y < H; p.y++)
            for (p.x = 0; p.x < W; p.x++)
                _patch.set(p, _grid.get(p + _at));
    }

    bool set_patch_data(const grid_t &source, const point2i &offset = point2i(0, 0))
    {
        for (point2i p(0, 0); p.y < H; p.y++)
            for (p.x = 0; p.x < W; p.x++)
                _patch.set(p, source.get(p + offset));
        return false;
    }
    
    bool and_patch_data(const grid_t &source, const point2i &offset = point2i(0, 0))
    {
        for (point2i p(0, 0); p.y < H; p.y++)
            for (p.x = 0; p.x < W; p.x++)
                _patch.set(p, source.get(p + offset) & _patch.get(p));
        return false;
    }
    
    bool or_patch_data(const grid_t &source, const point2i &offset = point2i(0, 0))
    {
        for (point2i p(0, 0); p.y < H; p.y++)
            for (p.x = 0; p.x < W; p.x++)
                _patch.set(p, source.get(p + offset) | _patch.get(p));
        return false;
    }

    void set(const point2i &p, const T &value)
    {
        if (_in_patch(p)) {
            _patch_set(p, value);
            return;
        }
        _grid.set(p, value);
    }
    
    void print_in_patch(std::ostream &os)
    {
        for (point2i p(0, 0); p.y < size().h; p.y++) {
            for (p.x = 0; p.x < size().w; p.x++) {
                std::cout << (_in_patch(p) ? "X" : ".");
            }
            std::cout << std::endl;
        }
    }

    T get(const point2i &p) const
    {
        return _in_patch(p) ? _patch_get(p) : _grid.get(p);
    }
    
    void resolve()
    {
        if (_isset) {
            for (point2i p(0, 0); p.y < H; p.y++) {
                for (p.x = 0; p.x < W; p.x++) {
                    _grid.set(p + _at, _patch.get(p));
                }
            }
            _isset = false;
        }
    }
    
    inline size2u size() const { return _grid.size(); }
       
private:

    inline T       _patch_get(const point2i &p)             const { return _patch.get(p - _at); }
    inline self_t &_patch_set(const point2i &p, const T &v)       { _patch.set(p - _at, v); return *this; }
    inline bool    _in_patch(const point2i &p)              const { point2i q(p - _at); return _isset && q.x >= 0 && q.x < W && q.y >= 0 && q.y < H; }
};

}
