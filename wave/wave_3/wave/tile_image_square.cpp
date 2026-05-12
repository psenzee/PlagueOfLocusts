#include "tile_image_strip.h"
#include "png_file.h"

tile_image_strip::tile_image_strip(const size2u &tile_size, std::size_t tile_count) :
    _data(nullptr), _size(tile_size), _count(tile_count)
{
    reset(tile_size, tile_count);
}

tile_image_strip::~tile_image_strip()
{
    reset(size2u(0, 0), 0);
}

void tile_image_strip::reset(const size2u &size, std::size_t count)
{
    if (size == _size && count == _count && _data) {
        for (std::size_t i = 0, sz = _size.area() * _count; i != sz; ++i) {
            _data[i] = 0;
        }
        return;
    }
    if (_data) {
        delete [] _data;
    }
    _size = size2u(0, 0);
    _count = _tile_stride = _length = 0;
    _data = nullptr;
    if (size.w != 0 && size.h != 0 && count != 0) {
        _size = size; _count = count;
        _tile_stride = _size.w * _size.h;
        _length = _tile_stride * _count;
        _data = new uint32_t [_length];
    }
}

void tile_image_strip::copy_tile_image(std::size_t i, uint32_t *to, std::size_t to_stride) const
{
    auto s = _size.w;
    for (auto b = tile_image(i), e = b + _tile_stride; b != e; b += s) {
        for (std::size_t n = 0; n != s; n++) {
            to[n] = b[n];
        }
        to += to_stride;
    }
}

void tile_image_strip::save_png(std::string_view filename) const
{
    write_png__rgba32(filename.data(), data(), _size.w, _size.h * _count);
}

const uint32_t *tile_image_strip::_tile_from_image(const image_t &image, std::size_t i) const
{
    auto tr = image.tiles_per_row, r = i / tr, c = i % tr, tw = _size.w;
    return image.source + r * image.stride * _size.h + c * tw;
}

void tile_image_strip::_copy_tile_from_image_to(const image_t &image, std::size_t i, uint32_t *to) const
{
    auto s = image.stride;
    for (auto b = _tile_from_image(image, i), e = b + _size.h * s; b != e; b += s) {
        for (std::size_t n = 0; n < _size.w; n++) {
            to[n] = b[n];
        }
        to += _size.w;
    }
}

void tile_image_strip::_copy_image_tiles(const image_t &image)
{
    for (std::size_t i = 0, sz = std::min(image.count, count()); i < sz; i++) {
        _copy_tile_from_image_to(image, i, tile_image(i));
    }
}

void tile_image_strip::load_from_png(std::string_view filename)
{
    image_t image;
    read_png__rgba32(filename.data(), &image.source, &image.stride, &image.height);
    image.tiles_per_row = image.stride / _size.w;
    printf("tiles per row %u\n", uint32_t(image.tiles_per_row));
    image.rows = image.height / _size.h;
    printf("rows %u\n", uint32_t(image.rows));
    image.count = image.tiles_per_row * image.rows;
    printf("count %u\n", uint32_t(image.count));
    if (_count <= image.count) {
        reset(_size, image.count + 1);
    }
    _copy_image_tiles(image);
}
