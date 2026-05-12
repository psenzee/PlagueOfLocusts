#pragma once

#include <iostream>
#include <cstdint>
#include <cstring>
#include "grid.h"
#include "png_file.h"
#include "tileset_linear.h"

struct tileset
{
    tileset(const char *filename, const size2u &tile_size);

    inline const size2u     &size()                    const { return _tile_size; }
    inline const size2u     &image_size()              const { return _image_size; }
    
    inline const uint32_t   *data()                    const { return _data; }
    inline const std::size_t stride()                  const { return _image_size.w; }
    inline std::size_t       count()                   const { return _count; }
    
    inline const uint32_t   *tile(std::size_t i) const
    {
        auto tr = _tiles_per_row, r = i / tr, c = i % tr, tw = _tile_size.w;
        return _data + r * stride() * _tile_size.h + c * tw;
    }
    
    inline void              copy_tile_to(std::size_t i, uint32_t *to) const
    {
        auto str = stride();
        for (auto b = tile(i), e = b + _tile_size.h * str; b != e; b += str) {
            for (std::size_t n = 0; n < _tile_size.w; n++) {
                to[n] = b[n];
            }
            to += _tile_size.w;
        }
    }
    
    void copy_to(tileset_linear &tsl)
    {
        for (std::size_t i = 0, sz = std::min(count(), tsl.count()); i < sz; i++) {
            copy_tile_to(i, tsl.tile(i));
        }
    }
    
    void save_tile_as_png(std::size_t i, const char *filename) const;
    
private:

    uint32_t    *_data;
    size2u       _image_size;
    size2u       _tile_size;
    std::size_t  _tiles_per_row, _rows, _count;
};
