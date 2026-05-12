//
//  main.cpp
//  wave
//
//  Created by paul on 12/27/23.
//

#include <random>
#include "world.h"
#include "std.h"
#include "u5.h"
#include "core_bits.h"

#include <string>
#include <string_view>
#include <algorithm>
#include <utility>
#include <array>
#include <iostream>
#include <span>

#include "ValueType.h"
#include "VertexFormats.h"

int main(int argc, const char * argv[])
{
    test_value_type();
    
    //test_vertex_format();
    test_vertex_formats();
    
    test_bit_reverse();

    const char *PATH = "/Users/Paul/Desktop/Projects/ultima_generate";

    std::mt19937 mt(100);

    size2u sz(2048, 2048);
    point2i p(0, 0);

    double scale = 1.;//0.5;
    auto w = wave::world(sz, p, mt, scale);

    w.create_world();

    u5::u5(w._elevation, PATH);

    auto range = w.elevation_range();

    std::cout << "range " << range[0] << " - " << range[1] << std::endl;
    auto fn(filename(PATH, "nu5map", "png")), fn_gs(filename(PATH, "nu5map_gs", "png"));
    
    w.save_rgba_png(fn.c_str());
    w.save_height_map_png(fn_gs.c_str());

    return 0;
}
