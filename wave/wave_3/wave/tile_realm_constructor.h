#include <string>
#include <string_view>
#include <algorithm>
#include <utility>
#include <array>
#include <iostream>
#include <span>

#include "grid.h"
#include "grid_extra.h"
#include "map_data.h"
#include "tile_image_strip.h"
#include "binary_streams.h"
#include "image_rgba32.h"
#include "tile_level.h"
#include "tile_map.h"
#include "tile_renderer.h"
#include "render_window.h"
#include "std.h"

struct tile_realm_constructor
{
    tile_realm_constructor();
    
    tile_realm_constructor &set_tile_size_count(const size2u &size, std::size_t count);
    tile_realm_constructor &set_map_name(std::string_view name);
    std::size_t             add_level(const size2u &size);
    std::size_t             add_level(const region &region);
    tile_realm_constructor &fill_level(std::size_t level_id, std::size_t layer_id, uint32_t value);
    tile_realm_constructor &fill_level(std::size_t level_id, uint32_t value);
    tile_realm_constructor &fill_level(uint32_t value);
    tile_realm_constructor &set_level_name(std::size_t level_id, std::string_view name);
    tile_realm_constructor &set_level_name(std::string_view name);
    tile_realm_constructor &load_tile_images(const size2u &size, std::size_t max_count, std::string_view file);
    tile_realm_constructor &set_path(std::string_view path);
    tile_realm_constructor &save_tile_images_strip(std::string_view file);
    std::size_t             get_current_level_id();

    /*
    std::size_t data_write(binary_out_stream &s) const;
    std::size_t data_read(binary_in_stream &s);
    */

    tile_realm_constructor &render(const region &tile_region, const size2u &resolution);
    tile_realm_constructor &save_render_image(std::string_view file);

private:

    void _init_tile_set();

    tile_map          _map;
    tile_set          _set;
    tile_image_strip  _tile_images;
    std::string_view  _path;
    image_rgba32     *_image;
};
