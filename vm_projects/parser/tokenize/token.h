#pragma once

#include <string_view>

struct token
{
    enum Type {
        STRING_LITERAL,
        NUMBER_LITERAL,
        WORD,
        SYMBOL,
        SPACE,
        _MAX_TYPE
    };

    Type type;
    std::string_view s;
};
