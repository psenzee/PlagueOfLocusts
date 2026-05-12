#pragma once

#include <cstddef>
#include <array>
#include <string_view>
#include "grid.h"
#include "image_info.h"

struct image_rgba32
{
    typedef uint32_t             pixel_t;
    typedef image_info<uint32_t> info_t;
    
    image_rgba32(const size2u &size = size2u(0, 0));
    ~image_rgba32();
    
    void                   resize(const size2u &size);
    const pixel_t         *data()                               const;
    pixel_t               *data();
    bool                   read_png(std::string_view filename);
    bool                   save_png(std::string_view filename);
    inline const size2u   &size()                               const { return _size; }
    inline std::size_t     stride()                             const { return _size.w; }
    
    image_rgba32          *image_region(const region &r)        const;
    void                   clear(const pixel_t &value = pixel_t());
    
    info_t                 info(const region &r = region());

private:
    
    void _init(const size2u &size) const;
    void _copy_line_to(const point2i &at, std::size_t length, uint32_t *to) const;

    mutable uint32_t *_data;
    mutable size2u    _size;
};
