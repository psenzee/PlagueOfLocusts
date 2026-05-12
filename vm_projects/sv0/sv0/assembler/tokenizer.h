#pragma once

#include <iostream>
#include <iomanip>
#include <string_view>
#include "parsed_number.h"
#include "token.h"

template <typename IterT>
struct token_iterator
{
    IterT i, e;
    
    token_iterator(IterT begin, IterT end) : i(begin), e(end) {}
    
    const token *read_token()
    {
        const token *t = read_token_();
        if (t) {
            std::cout << *t;
        }
        return t;
    }

    const token *read_token_()
    {
        const token *t = nullptr;
        i = read_whitespace(i, e);
        if (i == e)              { return t; }
        if ((t = read_number())) { return t; }
        if ((t = read_symbol())) { return t; }
        return t;
    }
    
    bool is_end() const { return i >= e; }
    
private:

    token _token;
    
    static IterT read_whitespace(IterT i, IterT e)
    {
        i = read_space(i, e);
        while (*i == ';') { // read comment
            i = read_until(i, e, '\n');
            i = read_space(i, e);
        }
        return i;
    }
    
    const token *read_symbol()
    {
        i = read_whitespace(i, e);
        if (i  == e) { return nullptr; }
        IterT ni = read_lword(i, e);
        if (ni == i) { return nullptr; }
        _token._var = std::string(i, ni);
        i = ni;
        return &_token;
    }
    
    const token *read_number()
    {
        parsed_number n;
        auto at = n.read(i, e, true);
        if (at == i) {
            return nullptr;
        }
        i = at;
        _token._var = n;
        return &_token;
    }
};

template <typename IterT> auto make_token_iterator(IterT b, IterT e) { return token_iterator<IterT>(b, e); }
template <typename T>     auto make_token_iterator(const T &s)       { return token_iterator<typename T::const_iterator>(s.begin(), s.end()); }
