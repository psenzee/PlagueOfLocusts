#pragma once

#include <string_view>
#include <iostream>
#include "lex.h"
#include "pow10.h"

class parsed_number
{
    int64_t  _whole;
    uint64_t _fracn;
    uint16_t _fracd_exp;
    int16_t  _e;
    uint16_t _chars;
    int8_t   _sign;

public:
    
    parsed_number() : _whole(0), _fracn(0), _fracd_exp(0), _e(0), _chars(0), _sign(0) {}
    
    void clear() { _whole = 0; _fracn = 0; _fracd_exp = 0; _e = 0; _chars = 0; _sign = 0; }

    double double_value() const
    {
        double val(_whole);
        val += (_fracn / pow10::powd(_fracd_exp));
        if (_e) val *= pow10::powd(_e);
        return val;
    }
    
    int64_t int_value() const
    {
        if (is_int()) {
            if (!_e) return _whole;
            // todo watch for overflow here
            return _whole * pow10::powi(_e);
        }
        return static_cast<int64_t>(double_value());
    }

    bool is_negative_int() const { return is_int() && _sign < 0; }
    bool is_parsed()       const { return _chars; }
    bool is_int()          const { return _chars && !_fracn && !_e; }
    bool is_float()        const { return _chars && (_fracn || (_e > 0 && _e < 20)); }
    
    template <typename IterT> IterT read(IterT i, IterT e, bool allow_plus)
    {
        clear();
        auto b(i);
        _sign = 1;
        int16_t e_sign = 1;
        bool minus = *i == '-';
        if (minus || (allow_plus && *i == '+')) {
            _sign = minus ? -1 : 1;
            i++;
        }
        auto w = e;
        if (*i != '.') {
            int64_t n(0);
            auto b = read_uint(i, e, n);
            if (b != i) {
                w = i;
                _whole = n * _sign;
                i = b;
            } else {
                clear();
                return b; // no whole part -- not a number
            }
        }
        if (*i == '.') {
            i++;
            uint64_t n(0);
            auto b = read_uint(i, e, n);
            if (b != i) {
                _fracn = n;
                _fracd_exp = b - i;
                i = b;
            } else if (w == e) {
                clear();
                return b; // neither fractional nor whole part -- not a number
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
                _e = n * e_sign;
                i = b;
            }
        }
        _chars = std::distance(b, i);
        return i;
    }
    
    size_t min_bytes_required(bool require_signed = false) const
    {
        if (is_float()) {
            return sizeof(float);
        }
        if (is_int()) {
            constexpr const uint64_t sint_thresholds[] = { 0x7fffffff, 0x7fff, 0x7f, 0 },
                                     uint_thresholds[] = { 0xffffffff, 0xffff, 0xff, 0 };
            constexpr const size_t   sizes[]           = {          8,      4,    2, 1 };
            const uint64_t *thresholds = (require_signed || (_sign < 0)) ? sint_thresholds : uint_thresholds;
            int64_t abv = std::abs(int_value());
            for (size_t i = 0; i < 4; ++i) {
                if (abv > thresholds[i]) return sizes[i];
            }
        }
        return 0;
    }

    std::ostream &print(std::ostream &os) const
    {
        if (is_parsed()) {
            return os << std::dec
                      << double_value()
                      << " w: " << _whole << " fn: " << _fracn
                      << " fx: " << static_cast<int>(_fracd_exp) << " e: " << _e << " chars:" << _chars;
        }
        return os;
    }
};

inline std::ostream &operator<<(std::ostream &os, const parsed_number &num) { return num.print(os); }
