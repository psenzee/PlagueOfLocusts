#include "lex_numeric.h"
#include <string>
#include <iostream>

std::ostream &operator<<(std::ostream &os, const parsed_number &num)
{
    if (num.begin >= num.end) {
        return os << "";
    }

    if (num.sign < num.end) {
        os << *num.sign;
    }

    if (num.whole < num.whole_end) {
        os << std::string_view(num.whole, num.whole_end);
    }

    if (num.point < num.end) {
        os << *num.point;
    }

    if (num.fraction < num.fraction_end) {
        os << std::string_view(num.fraction, num.fraction_end);
    }

    if (num.e < num.end) {
        os << *num.e;
    }

    if (num.e_sign < num.end) {
        os << *num.e_sign;
    }

    if (num.exponent < num.exponent_end) {
        os << std::string_view(num.exponent, num.exponent_end);
    }
    
    os << " w: " << num.int_whole << " fn: " << num.uint_fraction_numerator
       << " f10: " << static_cast<int>(num.uint_fraction_pow10) << " e: " << num.int_exponent;

    return os;
}

void test_parse_number()
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
        auto e = sv.end();
        auto num = parse_number(sv.begin(), e, true);
        std::cout << "[" << num << "]" << std::endl;
    }
}
