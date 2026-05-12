#pragma once

#include "face.h"
#include <array>

template <std::size_t N>
struct face_n : public face
{
    void          push_back(const vertex &u)       { if (_count >= N) throw std::exception(); _vertices[_count++] = u; };
    const vertex &at(std::size_t i)          const { return _vertices[i]; }
    std::size_t   size()                     const { return _count; };
    
private:

    std::array<vertex, N> _vertices;
    std::size_t           _count;
};
