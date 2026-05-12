#pragma once

struct id_provider
{
    static const id_provider *instance()
    {
        if (!_instance) {
            // todo load persistent id
            _instance = new id_provider;
        }
        return _instance;
    }
    
    inline std::size_t next() const { return _id++; }
    
private:
    
    id_provider() : _id(0) {}
    
    mutable std::size_t _id;
};
