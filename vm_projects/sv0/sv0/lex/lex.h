#pragma once

#include <cctype>
#include <array>
#include <string_view>
#include <iostream>
#include "chars_utf8.h"

struct parse_error
{
    enum Error { ERROR_NONE, ERROR_UNTERMINATED_STRING_LITERAL };
};

typedef std::string_view::const_iterator      iter_t;
typedef std::pair<iter_t, parse_error::Error> iter_error_t;

template <typename IterT> inline IterT read_until(IterT i, IterT e, uint32_t until_char)
{
    for (; i < e && *i != until_char; ++i) ;
    return i < e ? i : e;
}

template <typename IterT> inline IterT skip_utf8(IterT i, IterT e)
{
    switch (*i & 0xf0) {
    case 0xc0:
    case 0xd0: i += 1; break;
    case 0xe0: i += 2; break;
    case 0xf0: i += 3; break;
    default: break;
    }
    return i >= e ? e : i;
}

template <typename IterT> inline IterT read_until_with_escaped(IterT i, IterT e, uint32_t until_char, uint32_t escape_char = '\\')
{
    for (; i < e && *i != until_char; ++i) {
        if (*i == escape_char) {
            i++;
        }
    }
    return i < e ? i : e;
}

template <typename IterT> inline IterT read_while_class(IterT i, IterT e, uint32_t cclass)
{
    for (; i < e && (chars_utf8::traits(*i) & cclass); ++i) ;
    return i < e ? i : e;
}

template <typename IterT> inline IterT read_until_class(IterT i, IterT e, uint32_t cclass)
{
    for (; i < e && !(chars_utf8::traits(*i) & cclass); ++i) ;
    return i < e ? i : e;
}

template <typename IterT> inline IterT read_digits(IterT i, IterT e)
{
    return read_while_class(i, e, chars_utf8::T_DIGIT);
}

template <typename IterT> inline IterT read_cword(IterT i, IterT e)
{
    if (!(chars_utf8::traits(*i) & chars_utf8::T_CWORD_START)) {
        return i;
    }
    return read_while_class(++i, e, chars_utf8::T_CWORD);
}

template <typename IterT> inline IterT read_lword(IterT i, IterT e)
{
    if (!(chars_utf8::traits(*i) & chars_utf8::T_LWORD_START)) {
        return i;
    }
    return read_while_class(++i, e, chars_utf8::T_LWORD);
}

template <typename IterT> inline IterT read_hex_digits(IterT i, IterT e)
{
    return read_while_class(i, e, chars_utf8::T_XDIGIT);
}

template <typename IterT> inline IterT read_space(IterT i, IterT e)
{
    return read_while_class(i, e, chars_utf8::T_SPACE);
}

template <typename IterT> inline std::pair<IterT, parse_error::Error> read_quoted_err(IterT i, IterT e, int quote_char = '"', int escape_char = '\\')
{
    typedef std::pair<IterT, parse_error::Error> iter_error_t;
    if (i == e || *i != quote_char) {
        return iter_error_t(i, parse_error::ERROR_NONE);
    }
    i = read_until_with_escaped(++i, e, quote_char, escape_char);
    if (i == e || *i != quote_char) {
        return iter_error_t(i, parse_error::ERROR_UNTERMINATED_STRING_LITERAL);
    }
    return iter_error_t(++i, parse_error::ERROR_NONE);
}

template <typename IterT> inline IterT read_quoted(IterT i, IterT e, int quote_char = '"', int escape_char = '\\')
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

template <typename IterT, typename IntT> inline IterT read_uint(IterT i, IterT e, IntT &n)
{
    n = IntT{};
    // todo what to do with overflow?
    for (; i < e && (chars_utf8::traits(*i) & chars_utf8::T_DIGIT); ++i) {
        n *= 10;
        n += *i - '0';
    }
    return i;
}

template <typename IterT, typename IntT> inline IterT read_int(IterT i, IterT e, IntT &n)
{
    IterT b(i);
    IntT sign{ 1 };
    if (*i == '-') {
        i++;
        sign = IntT(-1);
    }
    // todo what to do with overflow?
    i = read_uint(i, e, n);
    if (b == i) {
        n = IntT{};
        return i;
    }
    n *= sign;
    return i;
}

template <typename IterT, typename IntT> inline IterT read_hex_uint(IterT i, IterT e, IntT &n)
{
    n = IntT{};
    // todo what to do with overflow
    for (uint32_t cc = 0; i < e && ((cc = chars_utf8::traits(*i)) & chars_utf8::T_XDIGIT); ++i) {
        n <<= 4;
        int c = *i;
        if (cc & chars_utf8::T_DIGIT) {
            n += c - '0';
        } else if (cc & chars_utf8::T_UPPER) {
            n += c - 'A' + 10;
        } else {
            n += c - 'a' + 10;
        }
    }
    return i;
}

void test_read_space();
void test_read_quoted();
