#pragma once

#include <cstdint>
#include "grid.h"

struct tileset
{
    tileset(const size2u &size, std::size_t count);
    ~tileset();
    
    void load_from_png(const char *filename);
    
    inline const size2u     &size()                         const { return _size; }
    inline const uint32_t   *data()                         const { return _data; }
    inline const std::size_t stride()                       const { return _size[0]; }
    inline std::size_t       count()                        const { return _count; }
    inline const uint32_t   *tile(std::size_t i)            const { return _data + i * _tile_stride; }
    inline uint32_t         *tile(std::size_t i)                  { return _data + i * _tile_stride; }
    inline const std::size_t tile_stride()                  const { return _tile_stride; }
           void              save_png(const char *filename) const;

    inline void              copy_tile(std::size_t i, uint32_t *to, std::size_t to_stride) const
    {
        auto s = _size[0];
        for (auto b = tile(i), e = b + _tile_stride; b != e; b += s) {
            for (std::size_t n = 0; n != s; n++) {
                to[n] = b[n];
            }
            to += to_stride;
        }
    }
    
private:

    void            _init(const size2u &size, std::size_t count);
    
    struct tileset_image
    {
        uint32_t *source;
        std::size_t tiles_per_row, stride, height, rows, count;
    };

    const uint32_t *_tile_from_image(const tileset_image &image, std::size_t i)                       const;
    void            _copy_tile_from_image_to(const tileset_image &image, std::size_t i, uint32_t *to) const;
    void            _copy_image_tiles(const tileset_image &image);

    uint32_t    *_data;
    size2u       _size;
    std::size_t  _count, _length, _tile_stride;
};
