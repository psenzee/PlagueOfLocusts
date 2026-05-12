#include <cstdint>
#include "tile_renderer.h"
#include "render_window.h"
#include "tile_level.h"
#include "tile_set.h"
#include "tile_image_strip.h"

tile_renderer::tile_renderer(tile_level &level, tile_set &tiles, tile_image_strip &tile_images)
    : _level(level), _tiles(tiles), _tile_images(tile_images) {}

void tile_renderer::render(render_window &rw, const region &tile_region)
{
    if (tile_region.empty() || !tile_region.valid()) {
        return;
    }
    auto info = rw.target->info();
    point2i mn(tile_region.min), mx(tile_region.max);
    size2u tile_size = _tile_images.size();
    for (std::size_t layer = 0, layer_count = _level.layer_count(); layer < layer_count; ++layer) {
        const auto &grid = _level.layer(layer);
        for (point2i at(mn); at.y < mx.y; ++at.y) {
            for (at.x = mn.x; at.x < mx.x; ++at.x) {
                const auto tile_id = grid.get(at);
                if (tile_id) { // we don't render 0s
                    const auto t = _tiles.get(tile_id);
                    _render_one(rw, info, t, (at - mn) * tile_size);
                }
            }
        }
    }
}

void tile_renderer::_render_one(render_window &rw, image_rgba32::info_t info, const tile &t, const point2i &at)
{
    const auto tile_image_id = t.get_tile_image_id();
    _tile_images.copy_tile_image(tile_image_id, info.data + at.y * info.stride + at.x, info.stride);
}
