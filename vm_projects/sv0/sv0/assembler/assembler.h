#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <string_view>
#include <unordered_map>
#include <variant>
#include "program.h"
#include "tokenizer.h"
#include "code_stream.h"

typedef token_iterator<std::string_view::const_iterator> token_iterator_t;

struct assembler
{
    assembler() : _label_id(0), _errs(std::cerr) {}
   
    bool assemble(token_iterator_t &ti, program &p);

private:
    
    std::ostream &_error_unknown_symbol(const token &t);
    std::ostream &_error_message(std::string_view message);

    uint16_t _get_label_id(const std::string &label);
    
    bool _assemble_label(const std::string &sym, code_stream &os, code_element::Info info = code_element::LABEL);
    bool _assemble_jump(Code oc, token_iterator_t &ti, code_stream &os);
    bool _assemble_declare_label(const token &t, code_stream &os);
    bool _assemble_immediate(const token &t, code_stream &os);
    bool _assemble_explicit_immediate(Code opcode, token_iterator_t &ti, code_stream &os);
    bool _assemble_explicit_immediate(Code opcode, const token *cm, code_stream &os);
    bool _assemble_const_id(const token &t, code_stream &os);
    bool _assemble_one(token_iterator_t &ti, code_stream &os);

    uint16_t                                   _label_id, _pre_label_id;
    std::unordered_map<std::string, uint16_t>  _labels;
    std::ostream                              &_errs;
    std::ostringstream                         _out;
};
