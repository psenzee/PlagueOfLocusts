#pragma once

#include <cstdint>
#include <bitset>
#include "place_info.h"

struct tile : public serializable
{
    enum Flag { LAND = 1, WATER = 2, WALKABLE = 3 };

    typedef std::bitset<64> flags_t;
    typedef uint32_t        tile_image_id_t;
    typedef uint32_t        tile_image_set_id_t;
    
    inline tile(tile_image_set_id_t tile_image_set_id = 0, tile_image_id_t tile_image_id = 0) :
        _tile_image_set_id(tile_image_set_id), _tile_image_id(tile_image_id), _flags(false) {}

    inline tile                &set_info(const place_info &info)                     { _info = info; return *this; }
    inline const place_info    &get_place_info()                               const { return _info; }
    inline place_info          &get_place_info()                                     { return _info; }
    inline tile_image_id_t      get_tile_image_id()                            const { return _tile_image_id; }
    inline tile_image_set_id_t  get_tile_image_set_id()                        const { return _tile_image_set_id; }
    inline tile                &set_tile_image_set_id(tile_image_set_id_t tid)       { _tile_image_set_id = tid; return *this; }
    inline tile                &set_tile_image_id(tile_image_id_t tid)               { _tile_image_id = tid; return *this; }
    inline flags_t             &flags()                                              { return _flags; }
    inline const flags_t       &flags()                                        const { return _flags; }
    inline bool                 is_set(Flag flag)                              const { return _flags[flag]; }
    
    inline bool                 is_empty()                                     const { return _tile_image_set_id == 0 && _tile_image_id == 0; }

    std::size_t                 data_write(binary_out_stream &os)              const;
    std::size_t                 data_read(binary_in_stream &is);
    
private:

    tile_image_set_id_t _tile_image_set_id;
    tile_image_id_t     _tile_image_id;
    flags_t             _flags;
    place_info          _info;
};
