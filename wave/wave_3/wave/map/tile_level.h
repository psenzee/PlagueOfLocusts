#pragma once

#include <cstdint>
#include "grid.h"
#include "place_info.h"
#include "regions_info.h"
#include "binary_stream.h"
#include "tile_set.h"

struct tile_level : public serializable
{
    typedef uint32_t                            index_t;
    typedef std::vector<uint32_t>               ids_t;
    typedef grid::readable<index_t>             grid_t;
    typedef grid::basic<index_t>                basic_t;
    typedef grid::basic_layers<index_t>         layers_t;
    
    inline tile_level(const size2u &size = size2u(0, 0)) : _layers(size), _parent_region(point2i(0, 0), size) { _layers.add_layer(); }
    inline tile_level(const region &r) : _layers(r.size()), _parent_region(r) { _layers.add_layer(); }

    inline region              get_region()                      const { return region(point2i(0, 0), _layers.size()); }
    inline const regions_info &get_regions()                     const { return _regions; }
    inline regions_info       &get_regions()                           { return _regions; }
    inline tile_level         &set_parent_region(const region &r)      { _parent_region = r; return *this; }
    inline const region       &get_parent_region()                     { return _parent_region; }
    inline tile_level         &set_info(const place_info &info)        { _info = info; return *this; }
    inline const place_info   &get_info()                        const { return _info; }
    inline place_info         &get_info()                              { return _info; }
    
    inline const size2u       &size()                            const { return _layers.size(); }

    std::size_t                data_write(binary_out_stream &os) const;
    std::size_t                data_read(binary_in_stream &is);
    
    inline const layers_t     &layers(std::size_t i)             const { return _layers; }
    inline const grid_t       &layer(std::size_t i)              const { return _layers.layer(i); }
    inline basic_t            &layer(std::size_t i)                    { return _layers.layer(i); }
    inline std::size_t         layer_count()                     const { return _layers.count(); }
    
    void                       clear();
    
private:

    layers_t        _layers;
    region          _parent_region;
    place_info      _info;
    regions_info    _regions;
    
//  const tile_set *_tile_set;
};
