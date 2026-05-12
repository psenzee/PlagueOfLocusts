#pragma once

#include <array>

namespace wave {

template <typename T, size_t N>
struct fixed_list : public std::array<T, N>
{
    typedef std::array<T, N>                base_t;
    typedef typename base_t::const_iterator const_iterator_t;
    typedef typename base_t::iterator       iterator_t;
    
    size_t _count;
    
    inline fixed_list() : _count(0) {}
    
    inline void              push_back(const T &value)       { if (_count < std::array<T, N>::size()) (*this)[_count++] = value; }
    inline void              pop_back()                      { if (_count) _count--; }
    inline const_iterator_t  end()                     const { return base_t::begin() + _count; }
    inline iterator_t        end()                           { return base_t::begin() + _count; }
    inline size_t            size()                    const { return _count; }
    inline const T          &back()                    const { return (*this)[_count - 1]; }
    inline T                &back()                          { return (*this)[_count - 1]; }
};

}
