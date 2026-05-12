#pragma once

#include <cstddef>
#include "grid.h"

template <typename PixelT>
struct image_info
{
    PixelT      *data;
    size2u       size;
    std::size_t  stride;
};
