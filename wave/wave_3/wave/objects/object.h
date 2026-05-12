#pragma once

#include "serialize.h"

struct object
{
    enum Type : uint16_t { NONE = 0, COLLECTION, ITEM, AGENT };
    
    inline object(Type type = NONE) : _type(type) {}
    
    virtual ~object() {}
    
    inline Type type() const { return _type; }

    virtual std::size_t write(binary_out_stream &os)                      const = 0;
    virtual std::size_t read(binary_in_stream &is, bool read_type = true)       = 0;
    
private:
    
    Type _type;
};
