#pragma once

#include <cstdint>
#include <string_view>
#include "grid.h"
#include "tile_image_set.h"

struct tile_image_square: public tile_image_set
{
    tile_image_square(const size2u &tile_size = size2u(0, 0), std::size_t tile_count = 0);
    ~tile_image_square();
    
    inline const size2u     &size()                               const { return _size; }
    inline const uint32_t   *data()                               const { return _data; }
    inline const std::size_t stride()                             const { return _size.w; }
    inline std::size_t       count()                              const { return _count; }
    inline const uint32_t   *tile_image(std::size_t i)            const { return _data + i * _tile_stride; }
    inline uint32_t         *tile_image(std::size_t i)                  { return _data + i * _tile_stride; }
    inline const std::size_t tile_stride()                        const { return _tile_stride; }
    
    void                     reset(const size2u &size, std::size_t count);
    void                     save_png(std::string_view filename)                                 const;
    void                     load_from_png(std::string_view filename);
    void                     copy_tile_image(std::size_t i, uint32_t *to, std::size_t to_stride) const;
    
private:
    
    struct image_t
    {
        uint32_t *source;
        std::size_t tiles_per_row, stride, height, rows, count;
    };

    const uint32_t *_tile_from_image(const image_t &image, std::size_t i)                       const;
    void            _copy_tile_from_image_to(const image_t &image, std::size_t i, uint32_t *to) const;
    void            _copy_image_tiles(const image_t &image);

    uint32_t    *_data;
    size2u       _size;
    std::size_t  _count, _length, _tile_stride;
};
