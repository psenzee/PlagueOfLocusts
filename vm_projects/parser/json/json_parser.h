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
#include "json_keyword.h"

void generate_identifier_list();

typedef std::string_view::const_iterator sv_iter_t;

struct json_parser_delegate
{
    void start(json_constants::Type type)                          { std::cout << (type == json_constants::JSON_MAP ? "{" : "["); }
    void delimiter()                                               { std::cout << ','; }
    void separator()                                               { std::cout << ':'; }
    void end(json_constants::Type type)                            { std::cout << (type == json_constants::JSON_MAP ? "}" : "]"); }
    
    void string_literal(std::string_view s)                        { std::cout << s;  }
    void numeric_literal(std::string_view s, const core_number &n) { std::cout << n.double_value(); }
    void identifier_literal(json_constants::Type type)             { std::cout << json_constants::name(type); }
    
    template <typename IterT> void error(IterT start, IterT at, std::string_view message = "")
    {
        auto pos = position::from_iterators(start, at);
        std::cout << message << (message.empty() ? "" : " at ") << pos << std::endl;
    }
};

template <typename Delegate, typename IterT>
struct json_parser
{
    typedef Delegate                               delegate_t;
    typedef IterT                                  iterator_t;
    typedef json_keyword<IterT>                    keyword_t;

    delegate_t delegate;

    enum Error {
        ERROR_UNEXPECTED_CLOSING_BRACE, ERROR_UNEXPECTED_CLOSING_BRACKET, ERROR_UNEXPECTED_COLON, ERROR_UNEXPECTED_SYMBOL,
        ERROR_UNRECOGNIZED_IDENTIFIER, ERROR_UNTERMINATED_STRING_LITERAL, ERROR_UNKNOWN, _MAX_ERROR
    };

    static constexpr std::array<const char *, 6> ERROR_MESSAGES = {
        "unexpected closing brace",
        "unexpected closing bracket",
        "unexpected colon",
        "unexpected symbol",
        "unrecognized identifier",
        "unterminated string literal"
    };

    void parse(IterT start, IterT end)
    {
        static constexpr const size_t DEPTH = 256;
        lex_stack<json_constants::Type, DEPTH> types;
        static constexpr const json_constants::Type MAP   = json_constants::JSON_MAP,
                                                    ARRAY = json_constants::JSON_ARRAY;
        for (IterT i = start, e = end; i != e; ) {
            int ch = *i;
            auto traits = chars_utf8::traits(ch);
            if (traits & chars_utf8::T_SPACE) {
                i = read_space(i, e);
                continue;
            } else if (traits & chars_utf8::T_UTF8_START) {
                i = skip_utf8(i, e);
                continue;
            } else if (traits & chars_utf8::T_PUNCT) {
                switch (ch) {
                    case '{':
                        types.push(MAP);
                        delegate.start(MAP);
                        i++;
                        break;
                    case '}':
                        if (types.top() != MAP) {
                            _error(start, i, ERROR_UNEXPECTED_CLOSING_BRACE);
                            return;
                        }
                        delegate.end(types.pop());
                        i++;
                        break;
                    case '[':
                        types.push(ARRAY);
                        delegate.start(ARRAY);
                        i++;
                        break;
                    case ']':
                        if (types.top() != ARRAY) {
                            _error(start, i, ERROR_UNEXPECTED_CLOSING_BRACKET);
                            return;
                        }
                        delegate.end(types.pop());
                        i++;
                        break;
                    case ':':
                        if (types.top() != MAP) {
                            _error(start, i, ERROR_UNEXPECTED_COLON);
                            return;
                        }
                        delegate.separator();
                        i++;
                        break;
                    case ',':
                        delegate.delimiter();
                        i++;
                        break;
                    case '"':
                        {
                            auto k = read_quoted(i, e);
                            if (k == i) {
                                _error(start, i, ERROR_UNKNOWN);
                                return;
                            }
                            delegate.string_literal(std::string_view(i, k));
                            i = k;
                            if (i == e && *i != '"') {
                                _error(start, i, ERROR_UNTERMINATED_STRING_LITERAL);
                                return;
                            }
                        }
                        break;
                    default:
                        _error(start, i, ERROR_UNEXPECTED_SYMBOL);
                        return;
                }
            } else if (traits & chars_utf8::T_ALPHA) {
                auto pair = keyword_t::read(i, e);
                if (i == pair.first || pair.second == json_constants::JSON_UNKNOWN) {
                    _error(start, i, ERROR_UNRECOGNIZED_IDENTIFIER);
                    return;
                }
                i = pair.first;
                delegate.identifier_literal(pair.second);
            } else if (traits & chars_utf8::T_NUM_START) {
                auto pair = read_number(i, e, false);
                if (i == pair.first) {
                    _error(start, i, ERROR_UNKNOWN);
                    return;
                }
                delegate.numeric_literal(std::string_view(i, pair.first), pair.second);
            }
        }
    }

    void _error(IterT start, IterT current, Error error) { delegate.error(start, current, ERROR_MESSAGES[error]); }
};
