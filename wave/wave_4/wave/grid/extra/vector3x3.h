#pragma once

#include <iostream>
#include <iomanip>
#include "grid.h"

namespace grid {

template <typename T>
constexpr std::array<T, 9> window3x3(const readable<T> &a, const point2i &p)
{
    return std::array<T, 9>({
        a.get(p + point2i { -1, -1 }), a.get(p + point2i { 0, -1 }), a.get(p + point2i { 1, -1 }),
        a.get(p + point2i { -1,  0 }), a.get(p + point2i { 0,  0 }), a.get(p + point2i { 1,  0 }),
        a.get(p + point2i { -1,  1 }), a.get(p + point2i { 0,  1 }), a.get(p + point2i { 1,  1 }),
    });
}

template <typename T>
constexpr std::array<T, 2> vector3x3(const std::array<T, 9> &a)
{
    T dy = (a[0] - a[3]) + (a[3] - a[6])
         + (a[1] - a[4]) + (a[4] - a[7])
         + (a[2] - a[5]) + (a[5] - a[8])
         + (a[0] - a[4]) + (a[4] - a[8]) // diagonal
         + (a[2] - a[4]) + (a[4] - a[6]);
    T dx = (a[0] - a[1]) + (a[1] - a[2])
         + (a[3] - a[4]) + (a[4] - a[5])
         + (a[6] - a[7]) + (a[7] - a[8])
         + (a[0] - a[4]) + (a[4] - a[8]) // diagonal
         + (a[6] - a[4]) + (a[4] - a[2]);
    return std::array<T, 2>({ dx, dy });
}

template <typename T>
constexpr std::array<T, 2> vector3x3(const readable<T> &grid, const point2i &p)
{
    return vector3x3(window3x3(grid, p));
}

template <typename T>
constexpr unsigned direction3x3(const readable<T> &grid, const point2i &p)
{
    static const unsigned map[] = {
        directions::CENTER,    // 0
        directions::WEST,      // 1
        directions::EAST,      // 2
        directions::CENTER,    // 3 // both east and west?
        
        directions::NORTH,     // 4
        directions::NORTHWEST, // 5
        directions::NORTHEAST, // 6
        directions::NORTH,     // 7 // north + both east and west?
        
        directions::SOUTH,     // 8
        directions::SOUTHWEST, // 9
        directions::SOUTHEAST, // 10
        directions::SOUTH,     // 11 // south + both east and west?
        
        directions::CENTER,    // 12 // both north and south
        directions::WEST,      // 13 // west + both north and south
        directions::EAST,      // 14 // east + both north and south
        directions::CENTER,    // 15 // north + south + east + west
    };
    auto v = vector3x3(grid, p);
    return map[(v[0] < 0 ? 1 : 0) | (v[0] > 0 ? 2 : 0) | (v[1] < 0 ? 4 : 0) | (v[1] > 0 ? 8 : 0)];
}

}
