#include "utils.h"

std::ostream &print_hex(std::ostream &os, const std::string &s, char delimit)
{
    os << std::hex << std::setfill('0');
    char delim[2] = { delimit, 0 };
    for (auto ch : s) {
        os << delim << std::setw(2) << std::right << static_cast<int>(static_cast<uint8_t>(ch));
    }
    return os;
}
