#include "assembler.h"

bool assembler::assemble(token_iterator_t &ti, program &p)
{
    while (!ti.is_end()) {
        if (!_assemble_one(ti, p.data)) {
            break;
        }
    }
    p.bytecode = p.data.resolve();
    std::cout << "\ncount " << std::dec << p.bytecode.size() << "\n";
    return true;
}

std::ostream &assembler::_error_unknown_symbol(const token &t)
{
    return _error_message("unknown symbol '") << t << "'";
}

std::ostream &assembler::_error_message(std::string_view message)
{
    return _errs << "\nerror: " << message;
}

uint16_t assembler::_get_label_id(const std::string &label)
{
    if (_labels.contains(label)) {
        return _labels[label];
    }
    _labels[label] = ++_label_id;
    return _label_id;
}

bool assembler::_assemble_label(const std::string &sym, code_stream &os, code_element::Info info)
{
    os.write(Code::LABEL, code_element::OPCODE);
    os.write(_get_label_id(sym), info);
    return true;
}

bool assembler::_assemble_jump(Code oc, token_iterator_t &ti, code_stream &os)
{
    if (opcodes::is_jump(oc)) {
        const token *cm = ti.read_token();
        if (!cm) {
            _error_message("expected a label");
            return false;
        }
        os.write(oc, code_element::OPCODE);
        return _assemble_label(cm->as_symbol(), os, code_element::JMP_LABEL);
    }
    return false;
}

bool assembler::_assemble_declare_label(const token &t, code_stream &os)
{
    // label declarations start with ':'
    const std::string &s = t.as_symbol();
    if (s[0] != ':') {
        return false;
    }
    std::string sym(s.begin() + 1, s.end());
    return _assemble_label(sym, os);
}

bool assembler::_assemble_immediate(const token &t, code_stream &os)
{
    if (!t.is_number()) {
        return false;
    }
    os.write_immediate_from_token(t);
    return true;
}

bool assembler::_assemble_explicit_immediate(Code opcode, const token *cm, code_stream &os)
{
    if (!cm || !cm->is_number()) {
        _error_message("immediate opcode requires a literal value");
        return false;
    }
    os.write_opcode(opcode);
    if (opcodes::is_immediate_integer(opcode)) {
        int64_t val = cm->is_float() ? static_cast<int64_t>(cm->as_double()) : static_cast<int64_t>(cm->as_int());
        switch (opcode) {
        case Code::U8:    os.write(static_cast<uint8_t> (val)); break;
        case Code::I8:    os.write(static_cast<int8_t>  (val)); break;
        case Code::CONST: case Code::LIST:  case Code::LISTR:
        case Code::LOCAL: case Code::SET:   case Code::CALL:
        case Code::U16:   os.write(static_cast<uint16_t>(val)); break;
        case Code::I16:   os.write(static_cast<int16_t> (val)); break;
        case Code::U32:   os.write(static_cast<uint32_t>(val)); break;
        case Code::I32:   os.write(static_cast<int32_t> (val)); break;
        case Code::U64:   os.write(static_cast<uint64_t>(val)); break;
        case Code::I64:   os.write(static_cast<int64_t> (val)); break;
        default: /* todo: error */ break;
        }
    } else if (opcodes::is_immediate_float(opcode)) {
        float val = cm->is_float() ? cm->as_float() : static_cast<float>(cm->as_int());
        os.write(val);
    }
    return true;
}

bool assembler::_assemble_explicit_immediate(Code opcode, token_iterator_t &ti, code_stream &os)
{
    return _assemble_explicit_immediate(opcode, ti.read_token(), os);
}

const token *char_prefixed_token(const token &t, char start)
{
    if (!t.is_symbol_starting_with(start)) {
        return nullptr;
    }
    const std::string &s = t.as_symbol();
    auto ti = make_token_iterator(std::string_view(s.begin() + 1, s.end()));
    return ti.read_token();
}

bool assembler::_assemble_const_id(const token &t, code_stream &os)
{
    const token *num = char_prefixed_token(t, '$');
    if (!num || !num->is_integer()) {
        return false;
    }
    return _assemble_explicit_immediate(Code::CONST, num, os);
}

bool assembler::_assemble_one(token_iterator_t &ti, code_stream &os)
{
    const token *t = ti.read_token();
    if (!t) {
        return false;
    }
    if (t->is_number() && _assemble_immediate(*t, os)) {
        return true;
    } else if (t->is_symbol_starting_with('$')) {
        if (!_assemble_const_id(*t, os)) {
            _error_unknown_symbol(*t);
        }
        return true;
    } else if (t->is_symbol()) {
        if (_assemble_declare_label(*t, os)) {
            return true;
        }
        int code = opcodes::find(t->as_symbol().c_str());
        if (code == -1) {
            _error_unknown_symbol(*t);
            return false;
        }
        code_t opcode = static_cast<code_t>(code);
        Code oc = static_cast<Code>(opcode);
        if (opcodes::is_immediate(oc)) {
            return _assemble_explicit_immediate(oc, ti, os);
        }
        auto inf = opcodes::info(opcode);
        if (!inf) {
            _error_unknown_symbol(*t);
            return false;
        }
        if (_assemble_jump(oc, ti, os)) {
            return true;
        }
        os.write(opcode, code_element::OPCODE);
        return true;
    }
    _error_message("unknown error?");
    return false;
}
