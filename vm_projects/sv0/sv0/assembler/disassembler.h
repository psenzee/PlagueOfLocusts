#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <span>
#include "columnar.h"

class program;

class disassembler
{
public:
    
    static constexpr const std::array<int, 6> LAYOUT { -6, -5, -16, -12, -12, -32 };
    
    typedef std::string_view                    code_str_t;
    typedef typename code_str_t::const_iterator code_str_iter_t;
        
    disassembler(code_str_t code) : _columns(LAYOUT), _code(code), _at(_code.begin()), _offset(0), _end_offset(0) {}

    const std::string &next_line();
    disassembler &reset() { _offset = 0; _at = _code.begin(); return *this; }

    void disassemble(std::ostream &os);
    
    bool done() const { return _at >= _code.end(); }

private:

    code_str_t      _code;
    columnar        _columns;

    std::string     _line;
    size_t          _offset, _end_offset;
    code_str_iter_t _at;
    
    template <typename T> std::ostream &write_int(std::ostream &os, code_str_iter_t i, bool with_hex = true)
    {
        auto v = static_cast<int64_t>(*reinterpret_cast<const T *>(&*i));
        os << std::dec << v;
        return os;
    }

    template <typename T> std::ostream &write_int_hex(std::ostream &os, code_str_iter_t i, bool with_hex = true)
    {
        auto v = static_cast<int64_t>(*reinterpret_cast<const T *>(&*i));
        if (with_hex && std::abs(v) > 9) {
            os << "; " << (v < 0 ? "-" : "") << std::hex << "0x" << std::abs(v);
        }
        return os;
    }

    std::ostream &write_value(std::ostream &os, code_str_iter_t i, bool is_float, size_t size, bool is_signed);
    std::ostream &write_value_note(std::ostream &os, code_str_iter_t i, bool is_float, size_t size, bool is_signed);
    std::ostream &write_bytes(std::ostream &os, code_str_iter_t i, size_t size);
};
