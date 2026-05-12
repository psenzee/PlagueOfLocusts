#include "char_classes_generator.h"
#include "chars_utf8.h"

#include <cctype>
#include <iostream>
#include <iomanip>

char_classes_generator::array_t char_classes_generator::_data;

constexpr uint32_t char_classes_generator::_classes(int ch)
{
    uint32_t classes = 0;
    bool is_num_sign    = ch == '-' || ch == '+';
    bool is_num_start   = is_num_sign || ch == '.' || std::isdigit(ch);
    bool is_num         = is_num_start || std::isxdigit(ch) || ch == 'x';
    bool is_lword_start = std::strchr("!$%&*+-./:<=>?^_~|\@", ch) || std::isalpha(ch);
    bool is_lword       = is_lword_start || std::isdigit(ch) || ch == '#';
    classes |= is_num_start                  ? chars::T_NUM_START   : 0;
    classes |= is_num_sign                   ? chars::T_SIGN        : 0;
    classes |= is_num                        ? chars::T_NUM         : 0;
    classes |= ch > 127                      ? chars::T_HIGH        : 0;
    classes |= (ch & 0xc0) == 0xc0           ? chars::T_UTF8_START  : 0;
    classes |= (ch & 0xc0) == 0x80           ? chars::T_UTF8_NEXT   : 0;
    classes |= std::isalpha(ch)              ? chars::T_ALPHA       : 0;
    classes |= std::isupper(ch)              ? chars::T_UPPER       : 0;
    classes |= std::islower(ch)              ? chars::T_LOWER       : 0;
    classes |= std::isdigit(ch)              ? chars::T_DIGIT       : 0;
    classes |= std::isspace(ch)              ? chars::T_SPACE       : 0;
    classes |= std::isblank(ch)              ? chars::T_BLANK       : 0;
    classes |= std::isgraph(ch)              ? chars::T_GRAPH       : 0;
    classes |= std::isprint(ch)              ? chars::T_PRINT       : 0;
    classes |= std::iscntrl(ch)              ? chars::T_CNTRL       : 0;
    classes |= std::isxdigit(ch)             ? chars::T_XDIGIT      : 0;
    classes |= std::ispunct(ch)              ? chars::T_PUNCT       : 0;
    classes |= std::isalnum(ch) || ch == '_' ? chars::T_CWORD       : 0;
    classes |= std::isalpha(ch) || ch == '_' ? chars::T_CWORD_START : 0;
    classes |= is_lword                      ? chars::T_LWORD       : 0;
    classes |= is_lword_start                ? chars::T_LWORD_START : 0;
    return classes;
}

void char_classes_generator::initialize()
{
    if (_data[0] == 0) {
        for (int i = 0; i < SIZE; ++i) {
            _data[i] = _classes(i);
        }
    }
}

std::ostream &char_classes_generator::dump(std::ostream &os, const char *class_name)
{
    char_classes_generator::initialize();
    os << "const std::array<uint32_t, " << class_name << "::SIZE> " << class_name << "::data = {";
    for (size_t i = 0; i < 256; i++) {
        if (i % 8 == 0) {
            os << std::endl << "    ";
        }
        os << " " << std::setw(9) << _data[i] << ",";
    }
    os << std::endl;
    return os << "};" << std::endl;
}

std::ostream &char_classes_generator::generate(std::ostream &os)
{
    std::locale::global(std::locale("en_US.UTF-8"));
    char_classes_generator::initialize();
    return char_classes_generator::dump(os, "chars_utf8");
}
