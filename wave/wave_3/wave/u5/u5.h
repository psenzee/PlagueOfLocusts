#pragma once

#include <iostream>
#include <fstream>
#include <array>

#include "grid.h"
#include "std_array.h"
#include "tiles_u5.h"
#include "core_math.h"

namespace u5
{

using namespace wave; // TODO remove when namespaces are cleaned up

void u5(grid::grid<double> &elevation, const char *path)
{
    auto r = range(elevation);
    //std::cout << "range " << r << std::endl;
    size2u sz(elevation.size());
    grid::basic<size_t> quantized(sz);
    
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            quantized.set(p, tiles_u5<size_t>::tile_from_elevation(elevation.get(p), p));
        }
    }
    tiles_u5<size_t>::update(quantized);
    /*
    for (size_t passes = 0; passes < 2; passes++) {
        for (point2i p(0, 0); p.y < sz.h - 0; p.y++) {
            for (p.x = 0; p.x < sz.w - 0; p.x++) {
                quantized.set(p, tiles_u5<size_t>::get_shore_tile(quantized, p));
            }
        }
    }
    */
    std::ostringstream ostr;
    ostr << "function getMapData() {" << std::endl <<
    "    return [[" << sz.w << ", " << sz.h << ", " << sz.w << "], [" << std::endl;
    print_as_json(ostr, quantized, 2);
    ostr << "]];" << std::endl << "}" << std::endl;
    
    time_t t;
    time(&t);
    std::array<std::string, 2> fns({
        filename_append_id(path, "nu5map", "js", t),
        filename_append_id(path, "nu5map", "js")
    });
    for (const auto &fn : fns) {
        std::fstream s;
        s.open(fn.c_str(), std::ios::out);
        s << ostr.str();
        s.close();
        std::cout << "wrote to " << fn << std::endl;
    }
}

}
