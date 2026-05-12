#pragma once

#include <cstdint>
#include "grid.h"
#include "binary_stream.h"

namespace grid {

template <typename T>
struct basic_layers : public serializable
{
    typedef basic<T>             basic_t;
    typedef readable<T>          readable_t;
    typedef std::vector<basic_t> layers_t;
    
    inline basic_layers(const size2u &size = size2u(0, 0)) : _size(size) {}
    
    inline void add_layer(std::size_t count = 1)
    {
        for (std::size_t i = 0; i < count; ++i) {
            _layers.emplace_back(basic_t(_size));
        }
    }
    
    inline std::size_t        count()              const { return _layers.size(); }
    
    inline const layers_t    &layers()             const { return _layers; }
    inline const readable_t  &layer(std::size_t i) const { return _layers[i]; }
    inline       basic_t     &layer(std::size_t i)       { return _layers[i]; }
    inline const size2u      &size()               const { return _size; }
    
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
        clear();
        std::size_t layer_count = 0, bytes = is.read(layer_count);
        bytes += is.read(_size);
        add_layer(layer_count);
        for (auto &basic : _layers) {
            bytes += basic.data_read(is);
        }
        return bytes;
    }
    
    inline void clear(const size2u &size = size2u(0, 0)) { _size = size; _layers.clear(); }
    
private:
    
    size2u               _size;
    std::vector<basic_t> _layers;
};

}
