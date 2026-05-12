#include "disassembler.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>
#include "program.h"
#include "opcodes.h"
#include "columnar.h"
#include "code_stream.h"

std::ostream &disassembler::write_value(std::ostream &os, code_str_iter_t i, bool is_float, size_t size, bool is_signed)
{
    if (!is_float) {
        switch (size) {
        case 1: return is_signed ? write_int<int8_t >(os, i) : write_int<uint8_t >(os, i); break;
        case 2: return is_signed ? write_int<int16_t>(os, i) : write_int<uint16_t>(os, i); break;
        case 4: return is_signed ? write_int<int32_t>(os, i) : write_int<uint32_t>(os, i); break;
        case 8: return is_signed ? write_int<int64_t>(os, i) : write_int<uint64_t>(os, i); break;
        case 0: default: return os;
        }
    }
    return os << *reinterpret_cast<const float *>(&*i);
}

std::ostream &disassembler::write_value_note(std::ostream &os, code_str_iter_t i, bool is_float, size_t size, bool is_signed)
{
    if (!is_float) {
        switch (size) {
        case 1: return is_signed ? write_int_hex<int8_t >(os, i) : write_int_hex<uint8_t >(os, i); break;
        case 2: return is_signed ? write_int_hex<int16_t>(os, i) : write_int_hex<uint16_t>(os, i); break;
        case 4: return is_signed ? write_int_hex<int32_t>(os, i) : write_int_hex<uint32_t>(os, i); break;
        case 8: return is_signed ? write_int_hex<int64_t>(os, i) : write_int_hex<uint64_t>(os, i); break;
        case 0: default: return os;
        }
    }
    return os << "; " << *reinterpret_cast<const float *>(&*i);
}

std::ostream &disassembler::write_bytes(std::ostream &os, code_str_iter_t i, size_t size)
{
    os << std::hex;
    for (size_t j = 0; j < size; ++j) {
        os << std::setw(2) << std::setfill('0') << std::hex << static_cast<int>(static_cast<uint8_t>(*i++));
    }
    return os;
}

const std::string &disassembler::next_line()
{
    if (_at >= _code.end()) {
        return (_line = "\n");
    }
    std::ostringstream os;
    _line.clear();
    _offset = _at - _code.begin();
    // offset byte #
    _columns.stream(0) << std::dec << std::setfill('0') << std::setw(5) << _offset;
    _columns.stream(1) << std::hex << std::setfill('0') << std::setw(4) << _offset;
    Code ch = static_cast<Code>(*_at++);
    const opcodes::info_t *info = opcodes::info(ch);
    const char *code = info ? info->name : "<unknown>";
    uint8_t isz = opcodes::immediate_size(static_cast<Code>(ch));
    // opcode #
    _columns.stream(2) << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(static_cast<uint8_t>(ch)) << " ";
    write_bytes(_columns.stream(2), _at, isz);
    // opcode name
    _columns.stream(3) << code;
    if (isz) {
        // immediate value
        write_value     (_columns.stream(4), _at, opcodes::is_immediate_float(ch), isz, !opcodes::is_immediate_unsigned(ch));
        write_value_note(_columns.stream(5), _at, opcodes::is_immediate_float(ch), isz, !opcodes::is_immediate_unsigned(ch));
        _at += isz;
    }
    _columns.write(os << "\n");
    _columns.clear();
    _line = os.str();
    _end_offset = _at - _code.begin();
    return _line;
}

void disassembler::disassemble(std::ostream &os)
{
    reset();
    while (!done()) {
        os << next_line();
    }
}
