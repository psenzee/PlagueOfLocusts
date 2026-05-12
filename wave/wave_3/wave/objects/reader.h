#pragma once

#include "serialize.h"
#include "object.h"

struct reader
{
    typedef std::pair<std::size_t, std::unique_ptr<object> > size_object_t;
    
    size_object_t read(binary_in_stream &is) const;
    
    static reader *instance();
    
private:

    static reader *_instance;
};
