#pragma once

#include "vertex.h"
#include <vector>

struct face
{
    typedef std::array<float, 3> vec3_t;
    typedef std::array<float, 2> vec2_t;
    
    std::vector<vertex> vertices;
    vec3_t              normal;
};
