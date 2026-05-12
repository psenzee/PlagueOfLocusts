#pragma once

#include <cstddef>
#include <array>

template <typename T, size_t MAX>
struct lex_stack
{
    std::array<T, MAX> _data;
    size_t             _top;
    
    inline lex_stack() : _top(0) {}
    
    inline void     push(const T &v)       { _data[_top++] = v; }
    inline T        pop()                  { return _data[--_top]; }
    inline const T &top()            const { return _data[_top - 1]; }
};
