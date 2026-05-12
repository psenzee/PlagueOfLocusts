#pragma once

#include <cstdint>
#include "grid.h"
#include "region_info.h"

struct regions_info : public serializable
{
    typedef std::vector<region_info>                 regions_info_t;
    typedef std::vector<region_info>::const_iterator regions_iter_t;
    
    inline region          get_region()                                       const { return _region; }
    inline regions_info   &insert(const region_info &r)                             { _regions.push_back(r); return *this; }
    inline regions_iter_t  find_first(const point2i &p)                       const { return find(p, _regions.begin()); }
    inline regions_iter_t  find_next(const point2i &p, regions_iter_t begin)  const { return find(p, ++begin); }
    inline regions_iter_t  begin()                                            const { return _regions.begin(); }
    inline regions_iter_t  end()                                              const { return _regions.end(); }
    
    regions_iter_t         find(const point2i &p, regions_iter_t begin)       const;

    std::size_t            data_write(binary_out_stream &os)                  const;
    std::size_t            data_read(binary_in_stream &is);

private:

    bool           _valid;
    region         _region;
    regions_info_t _regions;
};
