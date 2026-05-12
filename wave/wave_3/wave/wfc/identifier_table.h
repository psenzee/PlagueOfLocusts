#pragma once

#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <ctime>
#include <queue>
#include <algorithm>
#include <cstring>
#include <bitset>

namespace wave {

template <std::size_t N>
struct identifier_table
{
    typedef std::bitset<N>           options_t;
    typedef std::array<options_t, N> data_t;
    
    data_t _data;
    
    inline identifier_table() : _data({{ 0 }}), _is_special({ 0 }) { _init(); }
    
    inline const options_t &get(size_t index) const { return _data[index]; }
    
    inline bool is_special(size_t index) const { return _is_special[index]; }
    
private:
    
    struct special_t
    {
        size_t id;
        size_t include[N];
    };
    
    constexpr static const special_t _specials[] = {
        { 112, { 4, 5, 6, 43, 44, 45, 0 } },                                  // grass, swamp, farm & small brush
        { 113, { 6, 8, 9, 10, 0 } },                                          // trees & brush
        { 114, { 14, 15, 0 } },                                               // foothills
        { 115, { 11, 12, 13, 0 } },                                           // mountains
        { 116, { 114, 115, 0 } },                                             // mountains and foothills
        { 117, { 112, 113, 116, 0 } },                                        // general terrain types
        { 118, { 32, 33, 34, 35, 36, 37, 38, 0 } },                           // path types
    };
    
    options_t _is_special;
    
    void _init()
    {
        for (size_t i = 0; i < N; i++) {
            _data[i].reset();
            _data[i].set(i);
        }
        if (true) {
            size_t count = sizeof(_specials) / sizeof(_specials[0]);
            for (size_t i = 0; i != count; i++) {
                const special_t &s = _specials[i];
                options_t op(0);
                _is_special.set(i);
                for (size_t j = 0; s.include[j]; j++)
                    op |= get(s.include[j]);
                _data[s.id] = op;
                std::cout << "identifier " << s.id << " count " << op.count() << std::endl;
            }
        }
    }
};

}
