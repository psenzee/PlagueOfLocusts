#pragma once

#include <bitset>
#include "grid.h"

namespace wave {

template <std::size_t BitCount, std::size_t PatchWidth, std::size_t PatchHeight>
struct wave_traits
{
    typedef std::bitset<BitCount>                                options_t;
    typedef std::array<std::size_t, BitCount>                    bit_list_t;

    typedef grid::grid<options_t>                                grid_t;
    typedef grid::readable<options_t>                            readable_t;
    typedef grid::basic<options_t>                               basic_t;
    typedef grid::fixed<options_t, PatchWidth, PatchHeight>      fixed_t;
    
    static constexpr auto bit_count()    { return BitCount; }
    static constexpr auto patch_width()  { return PatchWidth; }
    static constexpr auto patch_height() { return PatchHeight; }
};

}
