#pragma once

#include <vector>
#include <unordered_map>

#include "types.h"
#include "type_list.h"
#include "function.h"

class functions
{
    std::vector<function>                    _list;
    std::unordered_map<const char *, size_t> _map;
    
    uint32_t _add(const function &f) { size_t at = _list.size(); _list.push_back(f); return static_cast<uint32_t>(at); }
    int      _get(const char *name) { return _map.contains(name) ?  int(_map[name]) : -1;  }

public:

    const function &get(uint32_t id)       const { return _list[id]; }
    uint32_t        add(const function &f)       { int at = _get(f.name); return at == -1 ? _add(f) : uint32_t(at); }
};
