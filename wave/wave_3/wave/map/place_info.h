#pragma once

#include <string>
#include <string_view>
#include "binary_streams.h"

struct place_info : public serializable
{
    inline place_info(std::string_view name = "") : _name(name), _empty(name.empty()) {}
    
    inline std::string_view get_name()                           const { return _name; }
    inline place_info      &set_name(std::string_view name)            { _name = name; _empty = _name.empty(); return *this; }

    std::size_t             data_write(binary_out_stream &os)    const;
    std::size_t             data_read(binary_in_stream &is);

private:

    std::string _name;
    bool        _empty;
};
