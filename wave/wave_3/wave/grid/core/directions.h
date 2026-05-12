#pragma once

#include "point2i.h"

struct directions
{
    enum Direction {

        START  = 0,
        WEST   = 0, NORTHWEST = 1, NORTH = 2, NORTHEAST = 3,
        EAST   = 4, SOUTHEAST = 5, SOUTH = 6, SOUTHWEST = 7,
        CENTER = 8,

        MAX_DIRECTIONS
    };

    static constexpr point2i offset(const point2i &base, std::size_t dir) { return base + DIRECTIONS[dir % MAX_DIRECTIONS]; }
    static constexpr point2i offset(const point2i &base, Direction dir)   { return offset(base, static_cast<std::size_t>(dir)); }

private:

    static constexpr point2i DIRECTIONS[] =
    {
        point2i(-1,  0), point2i(-1, -1), point2i( 0, -1), point2i( 1, -1),
        point2i( 1,  0), point2i( 1,  1), point2i( 0,  1), point2i(-1,  1),
        point2i( 0,  0)
    };
};
