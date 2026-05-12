#pragma once

#include <vector>

#include "core_grid.h"
#include "utils.h"

namespace grid {

struct boolean : public grid<bool>
{
    typedef boolean self_t;

    std::vector<bool> _data;
    size2u            _size;
    bool              _null;
    region            _extrema;

    inline boolean() : _size(0, 0), _null(false) {}
    inline boolean(const size2u &sz) : _size(sz), _null(false) { init(sz); }
    inline boolean(const self_t &other) : _size(other._size), _null(other._null), _data(other._data) {}
    inline boolean(const readable<bool> &other) : _size(other.size()), _null(other.null_value()) { init(other.size()); copy(other, *this); }

    void init(const size2u &sz) { _size = sz; _null = false; _data.resize(_size.area()); clear(*this, false); _extrema.reset(); }

    inline self_t       &operator= (const self_t &other)            { if (this == &other) return *this; _size = other._size; _null = other._null; _data = other._data; _extrema = other._extrema; return *this; }
    inline bool          operator==(const self_t &other)      const { if (this == &other) return true; return _size == other._size && _data == other._data; }
    
    inline void          set(const point2i &p, const bool &v)       { _data[index(p, _size)] = v; if (v) _extrema.insert(normalize(p, _size)); else _extrema = ::grid::extrema(*this, _extrema); }
    inline bool          get(const point2i &p)                const { return _data[index(p, _size)]; }
    inline bool          null_value()                         const { return _null; }
    inline size2u        size()                               const { return _size; }
    inline const region &extrema()                            const { return _extrema; }
};

}
