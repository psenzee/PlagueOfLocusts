#include "lex_numeric.h"
#include "chars_utf8.h"
#include <string>
#include <iostream>
#include <charconv>

void test_read_number()
{
    std::array<const char *, 20> b = {
        "",
        ".",
        ".0",
        "-.0",
        "+.0",
        "234",
        "234.",
        "+234",
        "-234",
        "234 343",
        "234. 343",
        "234.343",
        "234.343 ",
        "-12321 ",
        "-133e13",
        "-133e13 ",
        "+133e13 ",
        "+133e-13 ",
        "+1.33e-13",
        "133.032e13 ",
    };
    std::cout << "=======" << std::endl;
    for (auto s : b) {
        std::string_view sv(s);
        auto b = sv.begin(), e = sv.end();
        auto num = read_number(b, e, true);
        int value = 0;
        auto r = std::from_chars(b, e, value);
        std::cout << "[" << num.second << " " << value << "]" << std::endl;
    }
}
