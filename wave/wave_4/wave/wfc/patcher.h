#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>

#include "grid.h"
#include "patch.h"

namespace wave {

template <typename T, size_t W, size_t H>
struct patcher : public grid::grid<T>
{
    typedef T                         options_t;
    typedef patcher<T, W, H>          self_t;
    typedef grid::grid<T>             grid_t;
    typedef grid::fixed<T, W, H>      patch_t;
    
    enum { OPTION_N = options_t::size() };

    grid_t  &_grid;
    patch_t  _patch;
    point2i  _at;
    bool     _isset;
    T        _null;

    inline patcher(grid_t &grid, const point2i &p) : _grid(grid), _isset(false) { patch(p); }
    inline patcher(grid_t &grid, T null_t) : _grid(grid), _isset(false), _null(null_t) {}
    inline patcher(const self_t &other) : _grid(other._grid), _isset(other._isset), _null(other._null), _at(other._at), _patch(other._patch) {}
    
    inline self_t &operator=(const self_t &other)               { if (this == &other) return *this; _grid = other._grid; _isset = other._isset; _null = other._null; _at = other._at; _patch = other._patch; return *this; }
    inline bool    operator==(const grid::grid<T> &other) const { if (this == &other) return true; _isset = false; return equal(*this, other); }

    void patch(const point2i &p)
    {
        _at = p - patch_t::CENTER;
        _isset = true;
        for (point2i p { 0, 0 }; p[1] < H; p[1]++)
            for (p[0] = 0; p[0] < W; p[0]++)
                _patch.set(p, _grid.get(p + _at));
    }

    bool set_patch_data(const grid_t &source, const point2i &offset = point2i { 0, 0 })
    {
        for (point2i p { 0, 0 }; p[1] < H; p[1]++)
            for (p[0] = 0; p[0] < W; p[0]++)
                _patch.set(p, source.get(p + offset));
        return false;
    }
    
    bool and_patch_data(const grid_t &source, const point2i &offset = point2i { 0, 0 })
    {
        for (point2i p { 0, 0 }; p[1] < H; p[1]++)
            for (p[0] = 0; p[0] < W; p[0]++)
                _patch.set(p, source.get(p + offset) & _patch.get(p));
        return false;
    }
    
    bool or_patch_data(const grid_t &source, const point2i &offset = point2i { 0, 0 })
    {
        for (point2i p { 0, 0 }; p[1] < H; p[1]++)
            for (p[0] = 0; p[0] < W; p[0]++)
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
        for (point2i p { 0, 0 }; p[1] < size().h; p[1]++) {
            for (p[0] = 0; p[0] < size().w; p[0]++) {
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
            for (point2i p { 0, 0 }; p[1] < H; p[1]++) {
                for (p[0] = 0; p[0] < W; p[0]++) {
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
    inline bool    _in_patch(const point2i &p)              const { point2i q(p - _at); return _isset && q[0] >= 0 && q[0] < W && q[1] >= 0 && q[1] < H; }
};

}
