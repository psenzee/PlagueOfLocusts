#pragma once

#include <unordered_map>
#include "tag.h"
#include "key.h"

class map
{
    std::unordered_map<key, tag> _map;

public:

    void       set(const key &k, const tag &value) { _map[k] = value; }
    const tag *get(const key &k)                   { return _map.contains(k) ? &_map[k] : nullptr; }
};
