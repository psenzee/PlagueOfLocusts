#pragma once

#include <cstdint>
#include "grid.h"
#include "place_info.h"
#include "binary_streams.h"

struct region_info : public serializable
{
    inline region_info(const region &r = region(), const place_info &info = place_info()) : _region(r), _info(info) {}

    inline region_info      &set_info(const place_info &info)        { _info = info; return *this; }
    inline const place_info &get_info()                        const { return _info; }
    inline place_info       &get_info()                              { return _info; }
    inline region_info      &set_region(const region &r)             { _region = r; return *this; }
    inline const region     &get_region()                      const { return _region; }
    inline region           &get_region()                            { return _region; }

    std::size_t              data_write(binary_out_stream &os) const;
    std::size_t              data_read(binary_in_stream &is);

private:

    region     _region;
    place_info _info;
};
