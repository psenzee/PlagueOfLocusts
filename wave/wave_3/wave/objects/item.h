#pragma once

#include "serialize.h"

struct item : public object
{
    enum ItemType { ITEM_NONE = 0 };
    
    inline item(ItemType item_type = ITEM_NONE) : object(ITEM) {}
    
    std::size_t write(binary_out_stream &os)                      const { return 0; }
    std::size_t read(binary_in_stream &is, bool read_type = true)       { return 0; }
};
