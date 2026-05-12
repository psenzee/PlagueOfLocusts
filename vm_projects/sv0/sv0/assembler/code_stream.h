#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include "token.h"
#include "code_element.h"
#include "opcodes.h"

struct code_stream
{
    std::ostream &hex;
    std::vector<code_element> stream;

    code_stream() : hex(std::cout) {}
    
    std::string resolve();

    template <typename T>
    code_stream &write(const T &value, code_element::Info info = code_element::NONE)
    {
        code_element elem(value);
        stream.push_back(elem.set_info(info));
        elem.value().print_hex(hex);
        return *this;
    }

    code_stream &write(Code value, code_element::Info info = code_element::OPCODE);
    
    template <typename T>
    code_stream &write_opcode_and_immediate_value(Code code, const T &value)
    {
        return write_opcode(code).write(value, code_element::DATA);
    }

    code_stream &write_opcode(Code value);
    code_stream &write_immediate_from_token(const token &t);

private:
    
    template <typename T> code_stream &write_numeric(const numeric &n) { return write(n.as<T>(), code_element::DATA); }
    
    code_stream &write_immediate_opcode_from_token(const token &t);
    code_stream &write_immediate_value_from_token(const token &t);
    void         set_label_offset(std::vector<uint16_t> &label_offsets, size_t id, int value);
    
    static Code _immediate_opcode_from_token(const token &t);
};
