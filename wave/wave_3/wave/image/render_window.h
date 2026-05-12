#pragma once

#include <cstdint>
#include "grid.h"

struct image_rgba32;

struct render_window
{
    image_rgba32 *target;
    region        window;
};
