#include "tileset.h"
#include "png_file.h"

tileset::tileset(const size2u &size, std::size_t count) :  _data(nullptr), _size(size), _count(count)
{
    _init(size, count);
}

tileset::~tileset()
{
    if (_data) {
        delete [] _data;
    }
}

void tileset::_init(const size2u &size, std::size_t count)
{
    if (_data) {
        delete [] _data;
    }
    _size = size; _count = count;
    _tile_stride = _size[0] * _size[1];
    _length = _tile_stride * _count;
    _data = new uint32_t [_length];
}

void tileset::save_png(const char *filename) const
{
    write_png__rgba32(filename, data(), _size[0], _size[1] * _count);
}

const uint32_t *tileset::_tile_from_image(const tileset_image &image, std::size_t i) const
{
    auto tr = image.tiles_per_row, r = i / tr, c = i % tr, tw = _size[0];
    return image.source + r * image.stride * _size[1] + c * tw;
}

void tileset::_copy_tile_from_image_to(const tileset_image &image, std::size_t i, uint32_t *to) const
{
    auto s = image.stride;
    for (auto b = _tile_from_image(image, i), e = b + _size[1] * s; b != e; b += s) {
        for (std::size_t n = 0; n < _size[0]; n++) {
            to[n] = b[n];
        }
        to += _size[0];
    }
}

void tileset::_copy_image_tiles(const tileset_image &image)
{
    for (std::size_t i = 0, sz = std::min(image.count, count()); i < sz; i++) {
        _copy_tile_from_image_to(image, i, tile(i));
    }
}

void tileset::load_from_png(const char *filename)
{
    tileset_image image;
    read_png__rgba32(filename, &image.source, &image.stride, &image.height);
    image.tiles_per_row = image.stride / _size[0];
    printf("tiles per row %u\n", uint32_t(image.tiles_per_row));
    image.rows = image.height / _size[1];
    printf("rows %u\n", uint32_t(image.rows));
    image.count = image.tiles_per_row * image.rows;
    printf("count %u\n", uint32_t(image.count));
    if (_count <= image.count) {
        _init(_size, image.count + 1);
    }
    _copy_image_tiles(image);
}
