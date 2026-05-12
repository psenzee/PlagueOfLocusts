#pragma once

#include <iostream>
#include <iomanip>

#include "obj_option.h"
#include "obj_vertex.h"
#include "indexer.h"
#include "vertex.h"

struct obj_index
{
    typedef std::array<float, 3> vec3_t;
    typedef std::array<float, 2> vec2_t;

    indexer<vec3_t> positions,
                    normals;
    indexer<vec2_t> uvs;
    
    inline obj_vertex insert(const vertex &v)
    {
        return obj_vertex(positions.insert(v.position),
                          normals.insert(v.normal),
                          uvs.insert(v.uv));
    }

    inline std::ostream &print(const obj_option &option, std::ostream &s) const
    {
        if (option.position) _print(s, option, "v ", positions.list());
        if (option.normal)   _print(s, option, "vn", normals.list());
        if (option.uv)       _print(s, option, "vt", uvs.list());
        return s;
    }
    
private:

    template <std::size_t N>
    static std::ostream &_print(std::ostream &s, const obj_option &option, const char *type, const std::vector< std::array<float, N> > &vecs)
    {
        for (const auto &v: vecs) {
            s << type;
            for (const auto &e: v) {
                s << " " << std::fixed << std::setprecision(int(option.precision)) << e;
            }
            s << "\n";
        }
        return s;
    }
};
