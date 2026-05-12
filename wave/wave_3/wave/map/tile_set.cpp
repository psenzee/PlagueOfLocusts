#include "tile_set.h"
#include "binary_streams.h"

void tile_set::clear()
{
    _tiles_lo.clear();
    _tiles_hi.clear();
}

const tile &tile_set::get(uint32_t i) const
{
    static const tile empty;
    if (i >= THRESHOLD) {
        return _tiles_hi.contains(i) ? _tiles_hi.at(i) : empty;
    }
    std::size_t sz = _tiles_lo.size();
    if (i >= sz) {
        return empty;
    }
    return _tiles_lo[i];
}

tile_set &tile_set::set(uint32_t i, const tile &tile)
{
    if (i >= THRESHOLD) {
        _tiles_hi[i] = tile;
        return *this;
    }
    std::size_t sz = _tiles_lo.size();
    if (i == sz) {
        _tiles_lo.push_back(tile);
    } else if (i > sz) {
        _tiles_lo.resize(i + 1);
        _tiles_lo[i] = tile;
    } else {
        _tiles_lo[i] = tile;
    }
    return *this;
}

std::size_t tile_set::data_write(binary_out_stream &s) const
{
    std::size_t bytes = ::data_write(s, _tiles_lo);
    bytes += s.write(_tiles_hi.size());
    for (const auto &pair : _tiles_hi) {
        s.write(pair.first);
        bytes = pair.second.data_write(s);
    }
    return bytes;
}

std::size_t tile_set::data_read(binary_in_stream &s)
{
    std::size_t bytes = ::data_read(s, _tiles_lo), size = 0;
    bytes = s.read(size);
    tile t;
    uint32_t n = 0;
    for (std::size_t i = 0; i < size; ++i) {
        bytes += s.read(n);
        bytes += t.data_read(s);
        _tiles_lo[n] = t;
    }
    return bytes;
}
