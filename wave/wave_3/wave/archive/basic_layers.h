#pragma once

#include <cstdint>
#include "grid.h"
#include "binary_stream.h"

template <typename T>
struct basic_layers : public serializable
{
    typedef grid::basic<T>    basic_t;
    typedef grid::readable<T> readable_t;
    
    inline basic_layers(const size2u &size = size2u(0, 0)) : _size(size) {}

    inline void add_layer(std::size_t count = 1)
    {
        for (std::size_t i = 0; i < count; ++i) {
            _layers.emplace_back(basic_t(_size));
        }
    }

    inline const readable_t &layer(std::size_t i) const { return _layers[i]; }
    inline       basic_t    &layer(std::size_t i)       { return _layers[i]; }

    std::size_t data_write(binary_out_stream &os) const
    {
        std::size_t bytes = os.write(_layers.size()) + os.write(_size);
        for (const auto &basic : _layers) {
            bytes += basic.data_write(os);
        }
        return bytes;
    }

    std::size_t data_read(binary_in_stream &is)
    {
        _clear();
        std::size_t layer_count = 0, bytes = is.read(layer_count);
        add_layer(layer_count);
        bytes += is.read(_size);
        for (const auto &basic : _layers) {
            bytes += basic.data_read(is);
        }
        return bytes;
    }

private:
    
    void _clear()
    {
        _size = size2u(0, 0);
        _layers.clear();
    }

    size2u               _size;
    std::vector<basic_t> _layers;
};
