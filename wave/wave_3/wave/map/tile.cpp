#include "tile.h"

std::size_t tile::data_write(binary_out_stream &s) const
{
    return s.write(_tile_image_set_id) +
           s.write(_tile_image_id) +
           s.write(_flags) +
           _info.data_write(s);
}

std::size_t tile::data_read(binary_in_stream &s)
{
    return s.read(_tile_image_set_id) +
           s.read(_tile_image_id) +
           s.read(_flags) +
           _info.data_read(s);
}
