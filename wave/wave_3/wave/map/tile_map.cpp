#include "tile_map.h"
#include "serialize.h"

void tile_map::clear()
{
    _levels.clear();
    _info = place_info();
}

std::size_t tile_map::data_write(binary_out_stream &s) const
{
    std::size_t bytes = _info.data_write(s);
    bytes += ::data_write(s, _levels);
    return bytes;
}

std::size_t tile_map::data_read(binary_in_stream &s)
{
    clear();
    std::size_t bytes = _info.data_read(s);
    bytes += ::data_read(s, _levels);
    return bytes;
}
