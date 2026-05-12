#pragma once

#include <cctype>
#include <array>
#include <string_view>
#include <iostream>
#include "chars_utf8.h"

struct json_constants
{
    enum Type {
        JSON_FILE, JSON_MAP, JSON_ARRAY, JSON_STRING, JSON_NUMBER,
        JSON_BOOL_TRUE, JSON_BOOL_FALSE, JSON_NULL, JSON_UNKNOWN, JSON_ERROR
    };
    
    static const char *name(Type type)
    {
        switch (type) {
        case JSON_FILE:       return "<file>";
        case JSON_MAP:        return "<map>";
        case JSON_ARRAY:      return "<array>";
        case JSON_STRING:     return "<string>";
        case JSON_NUMBER:     return "<number>";
        case JSON_BOOL_TRUE:  return "true";
        case JSON_BOOL_FALSE: return "false";
        case JSON_NULL:       return "null";
        case JSON_ERROR:      return "<error>";
        case JSON_UNKNOWN:
        default:              return "<unknown>";
        }
    }
};

template <typename IterT> struct json_keyword : public json_constants
{
    typedef std::pair<IterT, Type> val_t;

    static val_t read(IterT b, IterT e)
    {
        static constexpr const Type NONE = JSON_UNKNOWN;
        auto len = std::distance(b, e);
        if (len < 4) {
            return val_t(b, NONE);
        }
        auto i = b;
        if (len >= 5) {
            switch (*i++) {
            case 'f': return *i++ == 'a' && *i++ == 'l' && *i++ == 's' && *i++ == 'e' && !_word(i, e) ? val_t(i, JSON_BOOL_FALSE) : val_t(i, NONE);
            case 'n': return *i++ == 'u' && *i++ == 'l' && *i++ == 'l' && !_word(i, e) ? val_t(i, JSON_NULL) : val_t(i, NONE);
            case 't': return *i++ == 'r' && *i++ == 'u' && *i++ == 'e' && !_word(i, e) ? val_t(i, JSON_BOOL_TRUE) : val_t(i, NONE);
            default:  return val_t(i, NONE);
            }
        }
        // len == 4
        switch (*i++) {
        case 'n': return *i++ == 'u' && *i++ == 'l' && *i++ == 'l' ? val_t(i, JSON_NULL) : val_t(i, NONE);
        case 't': return *i++ == 'r' && *i++ == 'u' && *i++ == 'e' ? val_t(i, JSON_BOOL_TRUE) : val_t(i, NONE);
        default:  return val_t(i, NONE);
        }
    }

private:

    static bool _word(const IterT b, const IterT e) { return (b < e && (chars_utf8::traits(*b) & chars_utf8::T_CWORD)); }
};
