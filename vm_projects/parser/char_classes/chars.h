#pragma once

#include <cstdint>

struct chars
{
    static constexpr uint32_t SIZE = 1 << 8;
    
    enum Trait : uint32_t
    {
        T_HIGH  = 1 <<  0, T_ALPHA       = 1 <<  1, T_SPACE = 1 <<  2, T_BLANK      = 1 <<  3, T_DIGIT       = 1 <<  4,
        T_PUNCT = 1 <<  5, T_XDIGIT      = 1 <<  6, T_GRAPH = 1 <<  7, T_PRINT      = 1 <<  8, T_CNTRL       = 1 <<  9,
        T_SIGN  = 1 << 10, T_NUM_START   = 1 << 11, T_NUM   = 1 << 12, T_UTF8_START = 1 << 13, T_UTF8_NEXT   = 1 << 14,
        T_UPPER = 1 << 15, T_LOWER       = 1 << 16, T_QUOTE = 1 << 17, T_CWORD      = 1 << 18, T_CWORD_START = 1 << 19,
        T_LWORD = 1 << 20, T_LWORD_START = 1 << 21,
    };
};
