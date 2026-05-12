#pragma once

#include <cstdint>
#include <memory>
#include "tile_level.h"

struct tile_map : public serializable
{
    inline tile_level       &get_level(std::size_t i = 0)                     { return *_levels[i]; }
    inline std::size_t       add_level(std::unique_ptr<tile_level> lvl)       { std::size_t sz = _levels.size(); _levels.emplace_back(std::move(lvl)); return sz; }
    inline std::size_t       level_count()                              const { return _levels.size(); }
    inline tile_map         &set_info(const place_info &info)                 { _info = info; return *this; }
    inline const place_info &get_info()                                 const { return _info; }
    inline place_info       &get_info()                                       { return _info; }

    std::size_t              data_write(binary_out_stream &os) const;
    std::size_t              data_read(binary_in_stream &is);
    
    void                     clear();
    
private:

    std::vector< std::unique_ptr<tile_level> > _levels;
    place_info                                 _info;
};
