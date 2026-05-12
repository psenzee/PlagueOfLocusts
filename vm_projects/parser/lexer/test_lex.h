#include <cctype>
#include <array>
#include <string_view>
#include <iostream>
#include <cstdint>

#define _inline

struct character_classes
{
    static constexpr uint32_t SIZE = 1 << 8;
    
    enum Class : uint32_t {
        
        CC_HIGH       = 1 <<  0,  CC_ALPHA      = 1 <<  1,  CC_SPACE       = 1 <<  2,  CC_BLANK      = 1 <<  3,  CC_DIGIT   = 1 <<  4,
        CC_PUNCT      = 1 <<  5,  CC_XDIGIT     = 1 <<  6,  CC_GRAPH       = 1 <<  7,  CC_PRINT      = 1 <<  8,  CC_CNTRL   = 1 <<  9,
        CC_SIGN       = 1 << 10,  CC_CWORD      = 1 << 11,  CC_CWORD_START = 1 << 12,  CC_NUM_START  = 1 << 13,  CC_NUM     = 1 << 14,
        CC_UTF8_START = 1 << 15,  CC_UTF8_NEXT  = 1 << 16,  CC_UPPER       = 1 << 17,  CC_LOWER      = 1 << 18
    };

    static constexpr uint32_t data[SIZE] = {

            512,    512,    512,    512,    512,    512,    512,    512,    512,    524,    516,    516,    516,    516,    512,    512,
            512,    512,    512,    512,    512,    512,    512,    512,    512,    512,    512,    512,    512,    512,    512,    512,
            268,    416,    416,    416,    416,    416,    416,    416,    416,    416,    416,  26016,    416,  26016,  24992,    416,
          27088,  27088,  27088,  27088,  27088,  27088,  27088,  27088,  27088,  27088,    416,    416,    416,    416,    416,    416,
            416, 154050, 154050, 154050, 154050, 154050, 154050, 137602, 137602, 137602, 137602, 137602, 137602, 137602, 137602, 137602,
         137602, 137602, 137602, 137602, 137602, 137602, 137602, 137602, 137602, 137602, 137602,    416,    416,    416,    416,   6560,
            416, 285122, 285122, 285122, 285122, 285122, 285122, 268674, 268674, 268674, 268674, 268674, 268674, 268674, 268674, 268674,
         268674, 268674, 268674, 268674, 268674, 268674, 268674, 268674, 285058, 268674, 268674,    416,    416,    416,    416,    512,
          66049,  66049,  66049,  66049,  66049,  66053,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,
          66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,  66049,
          65805,  65953,  65953,  65953,  65953,  65953,  65953,  65953,  65953,  65953, 334211,  65953,  65953,  65953,  65953,  65953,
          65953,  65953,  65921,  65921,  65953, 334211,  65953,  65953,  65953,  65921, 334211,  65953,  65921,  65921,  65921,  65953,
         170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 170371,
         170371, 170371, 170371, 170371, 170371, 170371, 170371,  33185, 170371, 170371, 170371, 170371, 170371, 170371, 170371, 301443,
         301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443,
         301443, 301443, 301443, 301443, 301443, 301443, 301443,  33185, 301443, 301443, 301443, 301443, 301443, 301443, 301443, 301443,
    };
};

struct parse_error
{
    enum Error { ERROR_NONE, ERROR_UNTERMINATED_STRING_LITERAL };
};

typedef std::string_view::const_iterator      iter_t;
typedef std::pair<iter_t, parse_error::Error> iter_error_t;

inline iter_t read_until(iter_t i, iter_t e, int until_char)
{
    for (; i < e && *i != until_char; ++i) ;
    return i < e ? i : e;
}

inline iter_t read_until_escaped(iter_t i, iter_t e, int until_char, int escape_char = '\\')
{
    for (; i < e && *i != until_char; ++i) {
        if (*i == escape_char) {
            i++;
        }
    }
    return i < e ? i : e;
}

inline iter_t read_while_class(iter_t i, iter_t e, uint32_t cclass)
{
    for (; i < e && (character_classes::data[*i] & cclass); ++i) ;
    return i < e ? i : e;
}

inline iter_t read_until_class(iter_t i, iter_t e, uint32_t cclass)
{
    for (; i < e && !(character_classes::data[*i] & cclass); ++i) ;
    return i < e ? i : e;
}

inline iter_error_t read_quoted(iter_t i, iter_t e, int quote_char, int escape_char = '\\')
{
    if (i == e || *i != quote_char) {
        return iter_error_t(i, parse_error::ERROR_NONE);
    }
    i = read_until_escaped(++i, e, quote_char, escape_char);
    if (i == e || *i != quote_char) {
        return iter_error_t(i, parse_error::ERROR_UNTERMINATED_STRING_LITERAL);
    }
    return iter_error_t(i, parse_error::ERROR_NONE);
}

inline iter_t read_digits(iter_t i, iter_t e)
{
    return read_while_class(i, e, character_classes::CC_DIGIT);
}

inline iter_t read_hex_digits(iter_t i, iter_t e)
{
    return read_while_class(i, e, character_classes::CC_XDIGIT);
}

inline iter_t read_space(iter_t i, iter_t e)
{
    return read_while_class(i, e, character_classes::CC_SPACE);
}

template <typename IntT>
inline std::pair<iter_t, IntT> read_uint(iter_t i, iter_t e)
{
    IntT n{};
    // todo what to do with overflow?
    for (; i < e && (character_classes::data[*i] & character_classes::CC_DIGIT); ++i) {
        n *= 10; // I expect the compiler to do something like `n = (n << 3) + n + n;` if it's actually faster
        n += *i - '0';
    }
    return std::pair<iter_t, IntT>(i, n);
}

template <typename IntT>
inline std::pair<iter_t, IntT> read_int(iter_t i, iter_t e)
{
    iter_t b = i;
    IntT sign{ 1 };
    if (*i == '-') {
        i++;
        sign = IntT(-1);
    }
    // todo what to do with overflow?
    auto res = read_uint<IntT>(i, e);
    if (res.first == i) {
        res = { b, 0 };
        return res;
    }
    res.n *= sign;
    return res;
}

template <typename IntT>
inline std::pair<iter_t, IntT> read_hex_uint(iter_t i, iter_t e)
{
    IntT n{};
    // todo what to do with overflow
    for (uint32_t cc = 0; i < e && ((cc = character_classes::data[*i]) & character_classes::CC_XDIGIT); ++i) {
        n <<= 4;
        int c = *i;
        if (cc & character_classes::CC_DIGIT) {
            n += c - '0';
        } else if (cc & character_classes::CC_UPPER) {
            n += c - 'A' + 10;
        } else {
            n += c - 'a' + 10;
        }
    }
    return std::pair<iter_t, IntT>(i, n);
}

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
