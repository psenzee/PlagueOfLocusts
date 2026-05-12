#pragma once

#include <iostream>
#include <iomanip>

#include "obj_option.h"
#include "obj_vertex.h"

struct obj_face
{
    std::vector<obj_vertex> vertices;
    
    inline std::size_t insert(const obj_vertex &v)
    {
        if (vertices.empty() || (v != vertices.back() && v != vertices.front())) {
            vertices.push_back(v);
        }
        return vertices.size() - 1;
    }

    std::ostream &print(const obj_option &option, std::ostream &s) const
    {
        if (vertices.size() > 2) {
            s << "f";
            if (option.reverse) {
                auto i = vertices.rbegin(), e = vertices.rend();
                for (; i != e; ++i) {
                    (*i).print(option, s << " ");
                }
            } else {
                auto i = vertices.begin(), e = vertices.end();
                for (; i != e; ++i) {
                    (*i).print(option, s << " ");
                }
            }
            s << "\n";
        }
        return s;
    }
};
