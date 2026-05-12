#pragma once

#include <iostream>
#include <sstream>
#include <iomanip>
#include <variant>
#include "numeric.h"

struct code_element
{
    enum Info : uint8_t { NONE, OPCODE, DATA, DATA_TAG, JMP_LABEL, LABEL };

    code_element() : _info(NONE), _value(0ull), _offset(0) {}
    
    template <typename T> explicit code_element(const T &v, Info info = NONE) : _info(info), _value(v),  _offset(0) {}
 
    code_element(const code_element &e) : _info(e._info), _value(e._value), _offset(e._offset) {}
    
    code_element &operator=(const code_element &e) { if (this != &e) { _info = e._info; _value = e._value; _offset = e._offset; } return *this; }
    code_element &operator=(const numeric &u)      { _value = u; return *this; }
    
    template <typename T> code_element &operator=(const T &v) { _value = v; return *this; }

    const numeric   &value()              const { return _value; }
    Info             info()               const { return _info; }
    code_element    &set_info(Info info)        { _info = info; return *this; }
    size_t           offset()             const { return _offset; }
    code_element    &set_offset(size_t u)       { _offset = u; return *this; }

private:

    numeric   _value;
    Info      _info;
    uint16_t  _offset;
};
