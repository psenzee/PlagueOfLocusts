#pragma once

#include <iostream>
#include <iomanip>
#include <variant>
#include <string>
#include <string_view>
#include <cstdint>
#include "parsed_number.h"
#include "numeric.h"

struct token
{
    enum Type { STRING = 0, NUMBER };

    std::variant<
        std::string,
        parsed_number
    > _var;

    std::ostream         &print(std::ostream &os)          const;
    
    Type                  type()                           const { return static_cast<Type>(_var.index()); }
    size_t                size()                           const { return is(NUMBER) ? as_number().min_bytes_required() : 0; }

    bool                  is(Type t)                       const { return type() == t; }
    
    bool                  is_number()                      const { return is(NUMBER); }
    bool                  is_integer()                     const { return is_number() && as_number().is_int(); }
    bool                  is_float()                       const { return is_number() && as_number().is_float(); }
    bool                  is_symbol()                      const { return is(STRING); }

    const parsed_number  &as_number()                      const { return std::get<parsed_number>(_var); }
    const std::string    &as_symbol()                      const { return std::get<std::string>(_var); }
    int64_t               as_int()                         const { return is_number() ? as_number().int_value() : 0; }
    uint64_t              as_uint()                        const { return static_cast<uint64_t>(as_int()); }
    double                as_double()                      const { return is_number() ? as_number().double_value() : 0.0; }
    float                 as_float()                       const { return static_cast<float>(as_double()); }
    bool                  is_symbol_starting_with(char ch) const { return is(STRING) && as_symbol()[0] == ch; }

    numeric               get_numeric()                    const;
};

inline std::ostream &operator<<(std::ostream &os, const token &t) { return t.print(os); }
