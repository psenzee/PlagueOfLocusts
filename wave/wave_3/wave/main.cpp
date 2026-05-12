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
#include "identifier_table.h"
#include "patch.h"
#include "rule.h"
#include "rules.h"
#include "availability_list.h"

#include "grid.h"
#include "grid_extra.h"
#include "map_data.h"
#include "png_file.h"

#include "histogram.h"
#include "nrules.h"

#include <string>
#include <string_view>
#include <algorithm>
#include <utility>
#include <array>
#include <iostream>
#include <span>

#include <stdio.h>
#include <stdlib.h>
#include "tile_image_strip.h"
#include "binary_streams.h"
#include "image_rgba32.h"

#include "tile_level.h"
#include "tile_map.h"
#include "tile_renderer.h"
#include "render_window.h"
#include "collection.h"
#include "agent.h"
#include "reader.h"

#include "tile_realm.h"

void objects_test()
{
    collection coll;
    reader reader;
    agent agentsd;
}

void maps_test()
{
    tile_realm realm;
    realm.set_map_name("Orlando");
    realm.set_path("/Users/Paul/Desktop/Projects/ultima_generate/");
    realm.add_level(size2u(128, 128));
    realm.set_level_name("Overworld");
    realm.fill_level(2);
    realm.load_tile_images(size2u(32, 32), 384, "u5_tiles");
    realm.save_tile_images_strip("u5_tilestrip_4");
    realm.render(region(point2i(5, 5), size2u(21, 14)), size2u(640, 400));
    realm.save_render_image("rendered");
}

void grid3d_test()
{
    typedef std::array<double, 16> value_type;
    auto base = grid::basic<value_type>(size2u(128, 128));
    auto slice = grid::slice(base, 0);
    grid::clear(slice, 3.14);
}

template <typename T, typename U, std::size_t N>
void nrules_sample(wave::nrules<N, T> &rules, std::size_t w, std::size_t h, const U *data)
{
    rules.sample(grid::const_data<U>(size2u(w, h), data));
}

template <typename T, std::size_t N>
void nrules_test()
{
    wave::nrules<N, T> rules;
    std::cout << "rules size: " << sizeof(rules) << "\n\n";
    for (std::size_t i = 0; i < 3; ++i) { // weight these * 3
        nrules_sample(rules,  25,  25, map25x25_rivers_paths);
    }
    nrules_sample(rules,  25,  25, map25x25_0);
    nrules_sample(rules, 100, 100, map100x50_0);
    nrules_sample(rules, 100, 256, map100x256_0);
    nrules_sample(rules, 100, 256, map100x256_1);
    nrules_sample(rules, 100, 100, map100x100_2);
    nrules_sample(rules, 100, 100, map100x100_3);
    nrules_sample(rules, 100, 100, map100x100_4);
    rules.clear_under(0.05, 3);
    rules.print(std::cout);
}

/*
template <typename T> void write_grid(binary_out_stream &os, const grid::readable<T> &grid)
{
    size2u sz(grid.size());
    os.write(sz);
    for (point2i p(0, 0); p.y < sz.h; p.y++) {
        for (p.x = 0; p.x < sz.w; p.x++) {
            os.write(grid.get(p));
        }
    }
}
*/
int main(int argc, const char * argv[])
{
    maps_test();

    tile_image_strip tsl(size2u(16, 16), 384);
    tsl.load_from_png("/Users/Paul/Desktop/Projects/ultima_generate/u5_tiles_original.png");
    tsl.save_png("/Users/Paul/Desktop/Projects/ultima_generate/u5_tilestrip_2.png");

    const char *PATH = "/Users/Paul/Desktop/Projects/ultima_generate";
    
    auto fn      (filename_append_id(PATH, "nu5map",            "png")),
         fn_gs   (filename_append_id(PATH, "nu5map_gs",         "png")),
         fn_obj  (filename_append_id(PATH, "nu5map",            "obj")),
         fn_tiles(filename_append_id(PATH, "u5_tiles_original", "png"));
    
    uint32_t *tiles_image = nullptr;
    size_t width = 0, height = 0;
    read_png__rgba32(fn_tiles.c_str(), &tiles_image, &width, &height);
    std::cout << "tiles image width " << width << " x height " << height << "\n";

    std_array::test();
    grid3d_test();
    nrules_test<std::size_t, 128>();
    
    std::mt19937 mt(100);

    size2u sz(2048, 2048);
    point2i p(0, 0);

    std::cout << "wave\n";
    
    wave::identifier_table<256> it2;
    wave::patch<3, 3, 256> p2;
    wave::rule<256> r2;
    wave::rules<256> rs2;
    //wave::availability_list<256> al2;

    double scale = 1.;//0.5;
    auto w = wave::world(sz, p, mt, scale);

    w.create_world();

    u5::u5(w._elevation, PATH);

    auto range = w.elevation_range();

    std::cout << "range " << range[0] << " - " << range[1] << std::endl;
    
    w.save_rgba_png(fn.c_str());
    w.save_height_map_png(fn_gs.c_str());
    w.save_obj(fn_obj.c_str());

    return 0;
}
