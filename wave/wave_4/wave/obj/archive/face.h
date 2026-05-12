#pragma once

#include "vertex.h"
#include <vector>

struct face
{
    typedef std::array<float, 3> vec3_t;
    typedef std::array<float, 2> vec2_t;
    
    vec3_t normal;

    virtual void          push_back(const vertex &u)       = 0;
    virtual const vertex &at(std::size_t i)          const = 0;
    virtual std::size_t   size()                     const = 0;
};
