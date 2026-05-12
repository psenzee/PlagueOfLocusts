#include "tile_realm_constructor.h"

tile_realm_constructor::tile_realm_constructor() : _tile_images(size2u(16, 16), 384)
{
    _init_tile_set();
}

tile_realm_constructor &tile_realm_constructor::set_tile_size_count(const size2u &size, std::size_t count)
{
    _tile_images.reset(size, count);
    return *this;
}

tile_realm_constructor &tile_realm_constructor::set_map_name(std::string_view name)
{
    _map.get_info().set_name(name);
    return *this;
}

std::size_t tile_realm_constructor::add_level(const size2u &size)
{
    return _map.add_level(std::unique_ptr<tile_level>(new tile_level(size)));
}

std::size_t tile_realm_constructor::add_level(const region &region)
{
    return _map.add_level(std::unique_ptr<tile_level>(new tile_level(region)));
}

tile_realm_constructor &tile_realm_constructor::fill_level(std::size_t level_id, std::size_t layer_id, uint32_t value)
{
    grid::fill(_map.get_level(level_id).layer(layer_id), value);
    return *this;
}

tile_realm_constructor &tile_realm_constructor::fill_level(std::size_t level_id, uint32_t value)
{
    return fill_level(level_id, 0, value);
}

tile_realm_constructor &tile_realm_constructor::fill_level(uint32_t value)
{
    return fill_level(get_current_level_id(), value);
}

tile_realm_constructor &tile_realm_constructor::set_level_name(std::size_t level_id, std::string_view name)
{
    _map.get_level(level_id).get_info().set_name(name);
    return *this;
}

tile_realm_constructor &tile_realm_constructor::set_level_name(std::string_view name)
{
    return set_level_name(get_current_level_id(), name);
}

tile_realm_constructor &tile_realm_constructor::load_tile_images(const size2u &size, std::size_t max_count, std::string_view file)
{
    auto fn(filename_append_id(_path, file, "png"));
    _tile_images.reset(size, max_count);
    _tile_images.load_from_png(fn.string());
    return *this;
}

tile_realm_constructor &tile_realm_constructor::set_path(std::string_view path)
{
    _path = path;
    return *this;
}

tile_realm_constructor &tile_realm_constructor::save_tile_images_strip(std::string_view file)
{
    auto fn(filename_append_id(_path, file, "png"));
    _tile_images.save_png(fn.string());
    return *this;
}

std::size_t tile_realm_constructor::get_current_level_id()
{
    return _map.level_count() - 1;
}

/*
std::size_t tile_realm_constructor::data_write(binary_out_stream &s) const
{
    std::size_t bytes = _map.data_read(s);
    bytes += _set.data_read(s);
    return bytes;
}

std::size_t tile_realm_constructor::data_read(binary_in_stream &s)
{
    clear();
    std::size_t bytes = _info.data_read(s);
    bytes += ::data_read(s, _levels);
    return bytes;
}
*/

tile_realm_constructor &tile_realm_constructor::render(const region &tile_region, const size2u &resolution)
{
    if (!_image) {
        _image = new image_rgba32(resolution);
        _image->clear();
    }
    tile_renderer tr(_map.get_level(0), _set, _tile_images);
    render_window rw;
    rw.target = _image;
    tr.render(rw, tile_region);
    return *this;
}

tile_realm_constructor &tile_realm_constructor::save_render_image(std::string_view file)
{
    if (_image) {
        auto fn(filename_append_id(_path, file, "png"));
        _image->save_png(fn.string());
    }
    return *this;
}

void tile_realm_constructor::_init_tile_set()
{
    _set.clear();
    for (uint32_t i = 0; i < 256; i++) {
        tile t(i, i);
        _set.set(i, t);
    }
}
