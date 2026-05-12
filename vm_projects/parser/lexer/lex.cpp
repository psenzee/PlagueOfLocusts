#include "lex.h"
#include <string>
#include <iostream>

void test_read_space()
{
    std::array<const char *, 5> a = {
        "",
        " ",
        "  ",
        " \t \n",
        " \n x \t",
    };
    for (auto str : a) {
        std::string_view s(str);
        auto e = s.end(), i = read_space(s.begin(), e);
        std::cout << "[" << std::string_view(i, e) << "]" << std::endl;
    }
}

void test_read_string(int quote)
{
    std::array<const char *, 9> b = {
        ""
    };
    for (auto str : b) {
        std::string_view s(str);
        auto e = s.end(), i = read_until(s.begin(), e, '"');
        std::cout << "[" << std::string_view(i, e) << "]" << std::endl;
    }
}
