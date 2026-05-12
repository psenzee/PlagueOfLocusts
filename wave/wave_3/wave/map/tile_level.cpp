#include "tile_level.h"
#include <iostream>

void tile_level::clear()
{
    _parent_region = region();
    _info = place_info();
    _regions = regions_info();
    _layers.clear();
}

std::size_t tile_level::data_write(binary_out_stream &os) const
{
    std::size_t bytes = os.write(_parent_region);
    bytes += _info.data_write(os);
    bytes += _regions.data_write(os);
    bytes += _layers.data_write(os);
    std::cout << "level bytes written " << bytes << "\n";
    return bytes;
}

std::size_t tile_level::data_read(binary_in_stream &is)
{
    clear();
    std::size_t bytes = is.read(_parent_region);
    bytes += _info.data_read(is);
    bytes += _regions.data_read(is);
    bytes += _layers.data_read(is);
    std::cout << "level bytes read " << bytes << "\n";
    return bytes;
}
