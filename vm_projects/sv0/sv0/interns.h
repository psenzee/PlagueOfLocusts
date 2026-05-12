#pragma once

#include <unordered_map>
#include <vector>
#include "tag.h"
#include "key.h"

class interns
{
    typedef std::vector<key>                       key_list_t;
    typedef std::unordered_map<size_t, key_list_t> hash_table_t; // ultimately replace with a faster implementation
    
    key_list_t::const_iterator _find(const key_list_t &list, const key &search)
    {
        return std::find(list.begin(), list.end(), search);
    }
    
    hash_table_t _map;

public:

    const key &intern(const key &k)
    {
        auto &list = _map[k.hash];
        size_t sz = list.size();
        if (sz == 0 || (sz == 1 && k != list[0])) {
            list.push_back(k);
        } else {
            auto i = _find(list, k);
            if (i == list.end()) {
                list.push_back(k);
            }
        }
        return list.back();
    }
    const key &intern(const std::string &s) { return intern(key(s)); }
};
