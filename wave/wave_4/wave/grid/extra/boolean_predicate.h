#pragma once

#include <vector>

#include "grid.h"

namespace grid {

template <bool NOT = false>
struct boolean_predicate
{
    const readable<bool> &_data;
    inline boolean_predicate(const readable<bool> &data) : _data(data) {}
    inline bool operator()(const point2i &p) const { bool value = _data(p); return NOT ? !value : value; }
};

}
