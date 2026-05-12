#include <iostream>
#include "token.h"

std::ostream &token::print(std::ostream &os) const
{
    os << std::dec;
    if (is_symbol()) {
        return os << "(sym " << std::get<0>(_var) << ")";
    }
    auto num = get_numeric();
    switch (num.index()) {
    case 0:  return os << "(u"  << num.size() * 8 << " " << num.as<uint64_t>() << ")";
    case 1:  return os << "(i"  << num.size() * 8 << " " << num.as<int64_t>()  << ")";
    case 2:  return os << "(f " << num.as<double>() << ")";
    default: return os;
    }
}

namespace
{
numeric make_numeric(const parsed_number &n, bool is_signed)
{
    if (n.is_float()) {
        return numeric(static_cast<float>(n.double_value()));
    }
    int64_t v = n.int_value();
    switch (n.min_bytes_required()) {
    case 8:  return is_signed ? numeric(static_cast<int64_t>(v)) : numeric(static_cast<uint64_t>(v));
    case 4:  return is_signed ? numeric(static_cast<int32_t>(v)) : numeric(static_cast<uint32_t>(v));
    case 2:  return is_signed ? numeric(static_cast<int16_t>(v)) : numeric(static_cast<uint16_t>(v));
    case 1:  return is_signed ? numeric(static_cast<int8_t >(v)) : numeric(static_cast<uint8_t >(v));
    case 0:
    default: return numeric(0);
    }
}
}

numeric token::get_numeric() const
{
    const auto &n = std::get<1>(_var);
    return make_numeric(n, n.is_negative_int());
}
