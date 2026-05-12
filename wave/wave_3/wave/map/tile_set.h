#pragma once

#include <cstdint>
#include <vector>
#include <unordered_map>
#include "tile.h"

struct tile_set : public serializable
{
    enum { THRESHOLD = 16384 };
    
    const tile &get(uint32_t i) const;
    tile_set   &set(uint32_t i, const tile &tile);
    void        clear();
    
    std::size_t data_write(binary_out_stream &os) const;
    std::size_t data_read(binary_in_stream &is);

private:

    std::vector<tile>                  _tiles_lo;
    std::unordered_map<uint32_t, tile> _tiles_hi;
};
