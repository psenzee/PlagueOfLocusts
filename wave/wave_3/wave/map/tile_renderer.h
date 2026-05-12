#pragma once

#include <cstdint>
#include "grid.h"
#include "image_rgba32.h"

struct tile;
struct tile_set;
struct tile_level;
struct tile_image_strip;
struct render_window;

struct tile_renderer
{
    typedef uint32_t                            index_t;
    typedef grid::readable<index_t>             grid_t;
    typedef grid::basic<index_t>                basic_t;
    typedef grid::basic_layers<index_t>         layers_t;
    
    tile_renderer(tile_level &level, tile_set &tiles, tile_image_strip &tile_images);
    
    void render(render_window &rw, const region &tile_region);
    
private:
    
    void _render_one(render_window &rw, image_rgba32::info_t info, const tile &t, const point2i &at);
                
    tile_level       &_level;
    tile_set         &_tiles;
    tile_image_strip &_tile_images;
};
