#pragma once

#include <unordered_map>
#include "registrable.h"

struct registry
{
    typedef std::unordered_map<std::size_t, registrable *> map_t;
    typedef std::pair<std::size_t, registrable *>          pair_t;
    
    ~registry() { _unload_all(); }
    
    registrable *get(std::size_t registry_id)
    {
        if (_map.contains(registry_id)) {
            return _map[registry_id];
        }
        return nullptr;
    }
    
    bool insert(registrable *r)
    {
        auto rid = r->registry_id();
        if (_map.contains(rid)) {
            return false;
        }
        _map[rid] = r;
        r->set_registry(this);
        return true;
    }

    bool unload(std::size_t registry_id)
    {
        auto *r = get(registry_id);
        if (!r) {
            return false;
        }
        _map.erase(registry_id);
        r->complete_unload();
        r->set_registry(nullptr);
        delete r;
        return true;
    }
    
    bool unload(registrable *r)
    {
        return r ? unload(r->registry_id()) : false;
    }
    
private:
    
    map_t _map;
    
    bool _unload_all()
    {
        std::vector<registrable *> rs;
        for (auto p : _map) {
            rs.push_back(p.second);
        }
        for (auto *r : rs) {
            unload(r);
        }
    }
};
