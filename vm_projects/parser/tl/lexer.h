#pragma once

#include <string_view>
#include <iostream>
#include <sstream>
#include <array>
#include <string>
#include <cctype>
#include <algorithm>
#include <string_view>
#include "strs.h"

namespace tl
{

struct word_style_c
{
    bool is_word_start_char(int ch) const { return std::isalpha(ch) || ch == '_'; }
    bool is_word_char(int ch)       const { return is_word_start_char(ch) || std::isdigit(ch); }
};

struct word_style_lisp
{
    bool is_word_start_char(int ch) const { return std::strchr("!$%&*+-./:<=>?^_~\\@", ch) || std::isalpha(ch); }
    bool is_word_char(int ch)       const { return is_word_start_char(ch) || std::isdigit(ch) || std::strchr("|#", ch); }
};

template <typename WordStyle, typename UInt = uint8_t, size_t Size = 256>
struct word_style_lookup
{
    WordStyle style;
    std::array<UInt, Size> data;
    
    enum { WORD = 1, WORD_START = 2 };
    
    word_style_lookup(WordStyle style = WordStyle()) : data({ 0 }), style(style)
    {
        for (int i = 0; i < data.size(); i ++) {
            data[i] |= (style.is_word_char(i)       ? WORD       : 0) |
                       (style.is_word_start_char(i) ? WORD_START : 0);
        }
    }
    
    bool is_word_start_char(int ch) const { return ch < data.size() ? (data[ch] & WORD_START) : style.is_word_start_char(ch); }
    bool is_word_char(int ch)       const { return ch < data.size() ? (data[ch] & WORD)       : style.is_word_char(ch); }
};

typedef std::string_view::const_iterator iter_t;

template <typename IterT>
struct lexer
{
    template <typename WordStyle> IterT read_word(const WordStyle &style, IterT i, IterT e) const
    {
        if (!style.is_word_start_char(*i)) {
            return i;
        }
        i++;
        for (; i < e && style.is_word_char(*i); ++i) ;
        return i;
    }

    IterT read_space(IterT i, IterT e) const
    {
        for (; i < e && std::isspace(*i); ++i) ;
        return i < e ? i : e;
    }

    IterT read_until_with_escaped(IterT i, IterT e, uint32_t until_char, uint32_t escape_char = '\\') const
    {
        for (; i < e && *i != until_char; ++i) {
            if (*i == escape_char) {
                i++;
            }
        }
        return i < e ? i : e;
    }

    IterT read_quoted(IterT i, IterT e, int quote_char = '"', int escape_char = '\\') const
    {
        if (i == e || *i != quote_char) {
            return i;
        }
        i = read_until_with_escaped(++i, e, quote_char, escape_char);
        if (i == e && *i != quote_char) {
            return i; // this is an error -- unterminated
        }
        return ++i;
    }

    IterT read_double(IterT i, IterT e, double *value) const
    {
        IterT b = i;
        char *end = 0;
        *value = std::strtod(b, &end);
        i = end;
        return i;
    }
};

}
