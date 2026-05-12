#pragma once

struct directions
{
    enum Direction {
        START = 0,
        WEST = 0, NORTHWEST = 1, NORTH = 2, NORTHEAST = 3,
        EAST = 4, SOUTHEAST = 5, SOUTH = 6, SOUTHWEST = 7,
        CENTER = 8,
        MAX_DIRECTIONS
    };
};
