#pragma once

#include <string_view>
#include <iostream>
#include "lex.h"
#include "pow10.h"

struct core_number
{
    int64_t  whole;
    uint64_t fracn;
    uint16_t fracd_exp;
    int16_t  e;
    uint16_t chars;

    double double_value() const
    {
        double val(whole);
        val += (fracn / pow10::powd(fracd_exp));
        if (e) val *= pow10::powd(e);
        return val;
    }
    
    int64_t int_value() const
    {
        if (is_int()) {
            if (!e) return whole;
            // todo watch for overflow here
            return whole * pow10::powi(e);
        }
        return static_cast<int64_t>(double_value());
    }

    inline bool is_int()   const { return fracn == 0 && e == 0; }
    inline bool is_float() const { return fracn != 0 || (e > 0 && e < 20); }
};

template <typename IterT> inline std::pair<IterT, core_number> read_number(IterT i, IterT e, bool allow_plus)
{
    typedef std::pair<IterT, core_number> result_t;
    auto b(i);
    core_number base_num = { 0, 0, 0, 0, 0 }, num(base_num);
    int64_t sign = 1LL;
    int16_t e_sign = 1;
    bool minus = *i == '-';
    if (minus || (allow_plus && *i == '+')) {
        sign = minus ? -1LL : 1LL;
        i++;
    }
    auto w = e;
    if (*i != '.') {
        int64_t n(0);
        auto b = read_uint(i, e, n);
        if (b != i) {
            w = i;
            num.whole = n * sign;
            i = b;
        } else {
            return result_t(b, base_num); // no whole part -- not a number
        }
    }
    if (*i == '.') {
        i++;
        uint64_t n(0);
        auto b = read_uint(i, e, n);
        if (b != i) {
            num.fracn = n;
            num.fracd_exp = b - i;
            i = b;
        } else if (w == e) {
            return result_t(b, base_num); // neither fractional nor whole part -- not a number
        }
    }
    if (*i == 'e' || *i == 'E') {
        i++;
        bool minus = *i == '-';
        if (minus || *i == '+') {
            e_sign = minus ? -1 : 1;
            i++;
        }
        int16_t n(0);
        auto b = read_uint(i, e, n);
        if (b != i) {
            num.e = n * e_sign;
            i = b;
        }
    }
    num.chars = std::distance(b, i);
    return result_t(i, num);
}

inline std::ostream &operator<<(std::ostream &os, const core_number &num)
{
    if (num.chars) {
        return os << std::dec
                  << num.double_value()
                  << " w: " << num.whole << " fn: " << num.fracn
                  << " fx: " << static_cast<int>(num.fracd_exp) << " e: " << num.e << " chars:" << num.chars;
    }
    return os;
}

void test_read_number();
