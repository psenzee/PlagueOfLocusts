#pragma once

#include <cstdint>
#include <string_view>
#include "id_provider.h"

struct registry;

struct registrable
{
    registrable(std::string_view type = ""sv, bool read_only = true, std::size_t rid = id_provider::instance()->next()) :
        _registry(nullptr), _registry_id(rid), _type(type), _read_only(read_only) {}
    virtual ~registrable() {}
    
    virtual std::size_t       registry_id()                           const { return _registry_id; }
    virtual std::string_view  registry_type()                         const { return _type; }
    virtual bool              read_only()                             const { return _read_only; }

    virtual void              set_registry_id(std::size_t rid)              { _registry_id = rid; }
    virtual void              set_registry_type(std::string_view type)      { _type = type; }
    virtual void              set_read_only(bool read_only)                 { _read_only = read_only; }
    virtual void              set_registry(registry *r)                     { _registry = r; }
    virtual registry         *get_registry()                                { return _registry; }
    
    virtual bool              complete_load()                 = 0;
    virtual bool              complete_unload()               = 0;
    virtual bool              save()                          = 0;
    
private:

    registry         *_registry;
    std::size_t       _registry_id;
    std::string_view  _type;
    bool              _read_only;
};
