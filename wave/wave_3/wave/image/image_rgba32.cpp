#include "image_rgba32.h"
#include <cstddef>
#include <array>
#include <string_view>
#include "png_file.h"

image_rgba32::image_rgba32(const size2u &size) : _size(size), _data(nullptr)
{
}

void image_rgba32::clear(const pixel_t &value)
{
    auto *pd = data();
    for (std::size_t i = 0, sz = _size.area(); i < sz; ++i) {
        *pd++ = value;
    }
}

image_rgba32::~image_rgba32()
{
    if (_data) {
        delete [] _data;
    }
    _data = nullptr;
}

const uint32_t *image_rgba32::data() const
{
    if (!_data) {
        _init(_size);
    }
    return _data;
}
    
uint32_t *image_rgba32::data()
{
    if (!_data) {
        _init(_size);
    }
    return _data;
}

image_rgba32::info_t image_rgba32::info(const region &r)
{
    if (r.empty()) {
        return info_t { data(), r.size(), stride() };
    }
    if (r.valid()) {
        return info_t { data() + r.min.y * stride() + r.min.x, r.size().mins(_size), stride() };
    }
    return info_t { data(), size(), stride() };
}
    
bool image_rgba32::read_png(std::string_view filename)
{
    uint32_t *data = nullptr;
    bool result = read_png__rgba32(filename.data(), &data, &_size.w, &_size.h);
    if (!result) {
        return false;
    }
    _init(_size);
    for (std::size_t i = 0, sz = _size.area(); i != sz; i++) {
        _data[i] = data[i];
    }
    release_read_png_data(&data);
    return true;
}

bool image_rgba32::save_png(std::string_view filename)
{
    write_png__rgba32(filename.data(), data(), _size.w, _size.h);
    return true;
}

void image_rgba32::resize(const size2u &size)
{
    _init(size);
}

void image_rgba32::_copy_line_to(const point2i &at, std::size_t length, uint32_t *to) const
{
    std::size_t mn = std::min(length, stride());
    const uint32_t *source = data() + stride() * at.y + at.x;
    for (std::size_t i = 0; i < mn; ++i) {
        *to++ = *source++;
    }
}

image_rgba32 *image_rgba32::image_region(const region &r) const
{
    size2u sz(r.size());
    point2i p(r.min);
    image_rgba32 *image = new image_rgba32(sz);
    uint32_t *target = image->data();
    for (std::size_t y = 0; y < sz.h; y++) {
        _copy_line_to(p, sz.w, target);
        target += sz.w;
        p.y++;
    }
    return image;
}
    
void image_rgba32::_init(const size2u &size) const
{
    _size = size;
    if (_data) {
        delete [] _data;
    }
    _data = nullptr;
    std::size_t area = _size.area();
    if (area) {
        _data = new uint32_t [area];
    }
}
