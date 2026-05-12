#include "code_stream.h"
#include "utils.h"

void code_stream::set_label_offset(std::vector<uint16_t> &label_offsets, size_t id, int value)
{
    while (label_offsets.size() <= id) {
        label_offsets.push_back(0);
    }
    label_offsets[id] = value;
}

std::string code_stream::resolve()
{
    std::ostringstream out;
    std::vector<uint16_t> label_offsets;
    int offset = 0;
    for (auto &data : stream) {
        auto info = data.info();
        data.set_offset(offset);
        const auto &value = data.value();
        size_t size = value.size();
        switch (info)
        {
        case code_element::OPCODE:    size = (value.as<uint64_t>() == static_cast<uint64_t>(Code::LABEL)) ? 0 : size;            break;
        case code_element::LABEL:     size = 0; set_label_offset(label_offsets, value.as<uint64_t>(), offset - sizeof(int16_t)); break;
        case code_element::JMP_LABEL: size = sizeof(int16_t);                                                                    break;
        default:                                                                                                                 break;
        }
        offset += size;
    }
    for (auto &data : stream) {
        if (data.info() == code_element::JMP_LABEL) {
            const auto &value = data.value();
            int32_t absolute_offset = label_offsets[value.as<uint64_t>()];
            data = static_cast<int16_t>(absolute_offset - data.offset());
        }
    }
    std::cout << "\n";
    for (auto &data : stream) {
        auto info = data.info();
        const auto &value = data.value();
        // label markers should be skipped
        if (info == code_element::LABEL) {
            continue;
        } else if (info == code_element::OPCODE) {
            if (value.as<uint8_t>() == static_cast<uint8_t>(Code::LABEL)) {
                continue;
            }
        }
        value.print_hex(std::cout);
        value.write(out);
    }
    out << static_cast<int8_t>(0);
    print_hex(std::cout, out.str());
    return out.str();
}

code_stream &code_stream::write(Code value, code_element::Info info)
{
    return write(static_cast<uint8_t>(value), info);
}

code_stream &code_stream::write_opcode(Code value)
{
    return write(value, code_element::OPCODE);
}

code_stream &code_stream::write_immediate_opcode_from_token(const token &t)
{
    return write(_immediate_opcode_from_token(t), code_element::OPCODE);
}

code_stream &code_stream::write_immediate_value_from_token(const token &t)
{
    if (!t.is_number()) {
        return *this;
    }
    auto num = t.get_numeric();
    auto id = num.index();
    if (id == 2) {
        return write_numeric<float>(num);
    }
    switch (t.size()) {
    case 1:  return id == 0 ? write_numeric<uint8_t> (num) : write_numeric<int8_t> (num);
    case 2:  return id == 0 ? write_numeric<uint16_t>(num) : write_numeric<int16_t>(num);
    case 4:  return id == 0 ? write_numeric<uint32_t>(num) : write_numeric<int32_t>(num);
    case 8:  return id == 0 ? write_numeric<uint64_t>(num) : write_numeric<int64_t>(num);
    default: return *this;
    }
}

code_stream &code_stream::write_immediate_from_token(const token &t)
{
    return write_immediate_opcode_from_token(t).write_immediate_value_from_token(t);
}

Code code_stream::_immediate_opcode_from_token(const token &t)
{
    Code ints[]  = { Code::ERROR, Code::I8, Code::I16, Code::ERROR, Code::I32, Code::ERROR, Code::ERROR, Code::ERROR, Code::I64 },
         uints[] = { Code::ERROR, Code::U8, Code::U16, Code::ERROR, Code::U32, Code::ERROR, Code::ERROR, Code::ERROR, Code::U64 };
    if (!t.is_number()) {
        return Code::ERROR;
    }
    auto num = t.get_numeric();
    switch (num.index()) {
    case 0:  return uints[num.size()];
    case 1:  return ints[num.size()];
    case 2:  return Code::F32;
    default: return Code::ERROR;
    }
}
