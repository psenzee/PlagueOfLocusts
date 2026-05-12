#pragma once

#include <vector>

#include "grid.h"

namespace wave {

struct bool_grid : public grid<bool>
{
    typedef bool_grid self_t;

    std::vector<bool> _data;
    size2u            _size;
    bool              _null;
    region            _extrema;

    inline bool_grid() : _size(0, 0), _null(false) {}
    inline bool_grid(const size2u &sz) : _size(sz), _null(false) { init(sz); }
    inline bool_grid(const self_t &other) : _size(other._size), _null(other._null), _data(other._data) {}
    inline bool_grid(const readable_grid<bool> &other) : _size(other.size()), _null(other.null_value()) { init(other.size()); copy(other, *this); }

    void init(const size2u &sz) { _size = sz; _null = false; _data.resize(_size.area()); clear(*this, false); _extrema.reset(); }

    inline self_t       &operator= (const readable_grid<bool> &other)        { if (this == &other) return *this; init(other.size()); return *this; }
    inline self_t       &operator= (const self_t &other)                     { if (this == &other) return *this; _size = other._size; _null = other._null; _data = other._data; _extrema = other._extrema; return *this; }
    inline bool          operator==(const self_t &other)               const { if (this == &other) return true; return _size == other._size && _data == other._data; }
    inline bool          operator==(const readable_grid<bool> &other)  const { if (this == &other) return true; return _size == other.size() && equal(*this, other); }
    
    inline void          set(const point2i &p, const bool &value)            { _data[index(p, _size)] = value; if (value) _extrema.insert(normalize(p, _size)); else wave::extrema(_extrema, *this, region()); }
    inline bool          get(const point2i &p)                         const { return _data[index(p, _size)]; }
    inline bool          null_value()                                  const { return _null; }
    inline size2u        size()                                        const { return _size; }
    inline const region &extrema()                                     const { return _extrema; }
};

template <bool NOT = false>
struct bool_grid_predicate
{
    const readable_grid<bool> &_data;
    inline bool_grid_predicate(const readable_grid<bool> &data) : _data(data) {}
    inline bool operator()(const point2i &p) const { bool value = _data(p); return NOT ? !value : value; }
};

}
