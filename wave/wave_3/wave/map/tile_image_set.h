#pragma once

#include <cstdint>
#include <string_view>
#include "grid.h"

struct tile_image_set
{
    virtual ~tile_image_set() {}
    
    virtual const size2u     &size()                                                              const = 0;
    virtual const uint32_t   *data()                                                              const = 0;
    virtual const std::size_t stride()                                                            const = 0;
    virtual std::size_t       count()                                                             const = 0;
    virtual const uint32_t   *tile_image(std::size_t i)                                           const = 0;
    virtual uint32_t         *tile_image(std::size_t i)                                                 = 0;
    virtual const std::size_t tile_stride()                                                       const = 0;
    virtual void              copy_tile_image(std::size_t i, uint32_t *to, std::size_t to_stride) const = 0;
};
