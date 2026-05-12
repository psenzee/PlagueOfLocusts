#pragma once

#include <iostream>
#include <iomanip>

#include "obj_option.h"

struct obj_vertex
{
    std::size_t position,
                normal,
                uv;

    inline obj_vertex() : position(0), normal(0), uv(0) {}
    inline obj_vertex(size_t position, size_t normal, size_t uv) : position(position), normal(normal), uv(uv) {}
    
    inline bool operator==(const obj_vertex &v) const { return position == v.position; }
    inline bool operator!=(const obj_vertex &v) const { return position != v.position; }

    inline std::ostream &print(const obj_option &option, std::ostream &s) const
    {
        if (option.position) s << (unsigned int)(position + 1);
        s << "/";
        if (option.uv)       s << (unsigned int)(uv + 1);
        if (option.normal)   s << "/" << (unsigned int)(normal + 1);
        return s;
    }
};
