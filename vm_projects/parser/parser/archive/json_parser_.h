#pragma once

#include <iostream>
#include <cctype>
#include <array>
#include <string_view>
#include <clocale>
#include "position.h"
#include "chars_utf8.h"
#include "lex_stack.h"
#include "lex.h"
#include "lex_numeric.h"

typedef std::string_view::const_iterator sv_iter_t;

struct json_constants
{
    enum Type {
        JSON_FILE, JSON_MAP, JSON_ARRAY, JSON_STRING, JSON_NUMBER,
        JSON_BOOL_TRUE, JSON_BOOL_FALSE, JSON_NULL, JSON_ERROR
    };
};

struct json_parser_delegate
{
    void start(json_constants::Type type)      { std::cout << (type == json_constants::JSON_MAP ? "{" : "["); }
    void delimiter()                           { std::cout << ','; }
    void end(json_constants::Type type)        { std::cout << (type == json_constants::JSON_MAP ? "}" : "]"); }
    
    void string_literal(std::string_view sv)   { std::cout << "\"" << sv << "\"";  }
    void numeric_literal(const core_number &n) { std::cout << n.double_value(); }
    void bool_true_literal()                   { std::cout << "true"; }
    void bool_false_literal()                  { std::cout << "false"; }
    void null_literal()                        { std::cout << "null"; }
    
    void error(const position &pos, std::string_view message = "")
    {
        std::cout << message << (message.empty() ? "" : " at ") << pos << std::endl;
    }
};

template <typename Delegate>
struct json_parser
{
    typedef Delegate                                    delegate_t;
    typedef std::string_view::const_iterator            iterator_t;
    typedef std::pair<iterator_t, json_constants::Type> return_type_t;
    
    static const size_t SIZE = 256;

    const uint32_t                   *classes;
    delegate_t                        delegate;

    json_parser() : classes(character_classes::data)
    {
        //character_classes::initialize();
    }

    void parse(std::string_view sv, position &pos)
    {
        lex_stack<json_constants::Type, SIZE> types;
        for (iterator_t i = sv.begin(), e = sv.end(); i != e; ++i) {
            int ch = *i;
            auto cl = classes[ch];
            if (cl & character_classes::CC_SPACE) {
                if (ch == '\n') {
                    pos.new_line();
                } else {
                    pos.next();
                }
            } else if (cl & character_classes::CC_PUNCT) {
                static const json_constants::Type
                    type_map   = json_constants::JSON_MAP,
                    type_array = json_constants::JSON_ARRAY;
                switch (ch) {
                    case '{':
                        types.push(type_map);
                        delegate.start(type_map);
                        break;
                    case '}':
                        if (types.top() != type_map) {
                            delegate.error(pos, "unexpected closing brace");
                        }
                        delegate.end(types.pop());
                        break;
                    case '[':
                        types.push(type_array);
                        delegate.start(type_array);
                        break;
                    case ']':
                        if (types.top() != type_array) {
                            delegate.error(pos, "unexpected closing bracket");
                        }
                        delegate.end(types.pop());
                        break;
                    case ':':
                        if (types.top() != type_map) {
                            delegate.error(pos, "unexpected colon");
                        }
                        // ...
                        break;
                    case ',':
                        delegate.delimiter();
                        break;
                    case '"':
                        i = _parse_string_literal(i, e, pos);
                        break;
                    default:
                        delegate.error(pos, "unexpected symbol");
                        break;
                }
            } else if (cl & character_classes::CC_ALPHA) {
                auto pair = _parse_true_false_null(i, e, pos);
                i = pair.first;
                switch (pair.second) {
                case json_constants::JSON_BOOL_TRUE:  delegate.bool_false_literal(); break;
                case json_constants::JSON_BOOL_FALSE: delegate.bool_false_literal(); break;
                case json_constants::JSON_NULL:       delegate.null_literal(); break;
                default: delegate.error(pos, "unknown identifier");
                }
                
            } else if (is_numeric_start(ch, cl, pos)) {
                auto pair = _parse_numeric_literal(i, e, pos);
                //i = pair.first;
            }
        }
    }
    
    inline return_type_t _parse_true_false_null(iterator_t i, iterator_t e, position &pos)
    {
        iterator_t start = i;
        return_type_t error(start, json_constants::JSON_ERROR);
        constexpr auto word = character_classes::CC_CWORD;
        constexpr const char *message = "unrecognized identifier";
        switch (*i++) {
        case 't':
            if (*i++ != 'r' || *i++ != 'u' || *i++ != 'e' || (classes[*i] & word)) {
                delegate.error(pos, message);
                return error;
            }
            return return_type_t(i, json_constants::JSON_BOOL_TRUE);
        case 'f':
            if (*i++ != 'a' || *i++ != 'l' || *i++ != 's' || *i++ != 'e' || (classes[*i] & word)) {
                delegate.error(pos, message);
                return error;
            }
            return return_type_t(i, json_constants::JSON_BOOL_FALSE);
        case 'n':
            if (*i++ != 'u' || *i++ != 'l' || *i++ != 'l' || (classes[*i] & word)) {
                delegate.error(pos, message);
                return error;
            }
            return return_type_t(i, json_constants::JSON_NULL);
        default:
            break;
        }
        delegate.error(pos, message);
        return error;
    }
    
    inline iterator_t _parse_string_literal(iterator_t i, iterator_t e, position &pos)
    {
        if (*i == '"') {
            i++;
            pos.next();
        }
        iterator_t start = i;
        for (; i != e; ++i) {
            switch (*i) {
            case '\\':
                i++;
                if (*i == 'n') {
                    pos.new_line();
                } else {
                    pos.next();
                }
                break;
            case  '"':
                delegate.string_literal(std::string_view(start, i));
                pos.next();
                return ++i;
            }
        }
        delegate.error(pos, "unterminated string literal");
        return i;
    }
    
    inline bool is_numeric_start(int ch, uint32_t cl, position &pos)
    {
        if (cl & character_classes::CC_DIGIT) {
            return true;
        } else {
            switch (ch) {
            case '-':
            case '+':
            case '.': return true;
            default:  return false;
            }
        }
        return false;
    }

    inline iterator_t _parse_numeric_literal(iterator_t i, iterator_t e, position &pos)
    {
        return i; // for now
    }
};
