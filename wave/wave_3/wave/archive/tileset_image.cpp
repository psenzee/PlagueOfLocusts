#pragma once

#include "tileset.h"
#include "png_file.h"

tileset::tileset(const char *filename, const size2u &tile_size) :  _data(nullptr), _tile_size(tile_size), _image_size(0, 0)
{
    std::size_t w = 0, h = 0;
    read_png__rgba32(filename, &_data, &w, &h);
    _image_size.w = w; _image_size.h = h;
    _tiles_per_row = stride() / _tile_size.w;
    printf("tiles per row %u\n", uint32_t(_tiles_per_row));
    _rows = _image_size.h / _tile_size.h;
    printf("rows %u\n", uint32_t(_rows));
    _count = _tiles_per_row * _rows;
    printf("count %u\n", uint32_t(_count));
}
 
void tileset::save_tile_as_png(std::size_t i, const char *filename) const
{
    uint32_t *data = new uint32_t [_tile_size.w * _tile_size.h];
    copy_tile_to(i, data);
    write_png__rgba32(filename, data, _tile_size.w, _tile_size.h);
}
