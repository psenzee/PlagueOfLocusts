#pragma once

#include <string_view>
#include <iostream>

static constexpr uint64_t _pow10(unsigned exp) { return !exp ? 1ull : _pow10(exp - 1) * 10ull; }

typedef std::string_view::const_iterator iter_t;

struct parsed_number
{
    iter_t begin;
    
    iter_t sign;

    iter_t whole;
    iter_t whole_end;

    iter_t point;

    iter_t fraction;
    iter_t fraction_end;

    iter_t e;
    iter_t e_sign;
    
    iter_t exponent;
    iter_t exponent_end;
    
    iter_t end;
    
    int64_t  int_whole;
    uint64_t uint_fraction_numerator;
    uint8_t  uint_fraction_pow10;
    int16_t  int_exponent;
};

std::ostream &operator<<(std::ostream &os, const parsed_number &num);

inline iter_t parse_digits(iter_t i, iter_t e)
{
    for (; i < e && std::isdigit(*i); ++i) {}
    return i;
}

template <typename IntT>
inline std::pair<iter_t, IntT> parse_int(iter_t i, iter_t e)
{
    IntT n{};
    // todo what to do with overflow
    for (; i < e && std::isdigit(*i); ++i) {
        n *= 10; // I expect the compiler to do something like `n = (n << 3) + n + n;` if it's actually faster
        n += *i - '0';
    }
    return std::pair<iter_t, IntT>(i, n);
}

inline parsed_number parse_number(iter_t i, iter_t e, bool allow_plus = true)
{
    parsed_number base_num = { i, e, e, e, e, e, e, e, e, e, e, i, 0, 0, 0, 0 }, num(base_num);
    int64_t sign = 1ll;
    int16_t e_sign = 1;
    if (*i == '-' || (allow_plus && *i == '+')) {
        sign = *i == '-' ? -1ull : 1ull;
        num.sign = i++;
    }
    if (*i != '.') {
        auto iu = parse_int<int64_t>(i, e);
        if (iu.first != i) {
            num.whole = i;
            num.whole_end = iu.first;
            num.int_whole = iu.second * sign;
            i = iu.first;
        } else {
            return base_num;
        }
    }
    if (*i == '.') {
        num.point = i++;
        auto iu = parse_int<uint64_t>(i, e);
        if (iu.first != i) {
            num.fraction = i;
            num.fraction_end = iu.first;
            num.uint_fraction_numerator = iu.second;
            num.uint_fraction_pow10 = iu.first - i;
            i = iu.first;
        } else if (num.whole == e) {
            // neither fractional nor whole part -- not a number
            return base_num;
        }
    }
    if (*i == 'e' || *i == 'E') {
        num.e = i++;
        if (*i == '-' || *i == '+') {
            e_sign = *i == '-' ? -1 : 1;
            num.e_sign = i++;
        }
        auto iu = parse_int<int16_t>(i, e);
        if (iu.first != i) {
            num.exponent = i;
            num.exponent_end = iu.first;
            num.int_exponent = iu.second * e_sign;
            i = iu.first;
        }
    }
    num.end = i;
    return num;
}

void test_parse_number();
