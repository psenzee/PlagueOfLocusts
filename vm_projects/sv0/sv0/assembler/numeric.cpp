#include "numeric.h"
#include <algorithm>
#include <cstring>

namespace
{
template <size_t N> void copy(const numeric &num, uint8_t *into, size_t size = 0)
{
    auto v = std::get<N>(num.value());
    std::memcpy(into, &v, size ? std::min(size, sizeof(v)) : sizeof(v));
}
}

size_t numeric::bytes(uint8_t *into, size_t size) const
{
    size = size ? size : _size_of;
    float v = 0.f;
    switch (_value.index()) {
    case 0: copy<0>(*this, into, size); break;
    case 1: copy<1>(*this, into, size); break;
    case 2:
        v = static_cast<float>(std::get<2>(_value));
        std::memcpy(into, &v, sizeof(float));
        return sizeof(float);
    default:                            return 0;
    }
    return size;
}

numeric::bytes_t numeric::bytes(size_t size) const
{
    bytes_t data { 0 };
    bytes(data.data(), size);
    return data;
}

std::ostream &numeric::print(std::ostream &os) const
{
    os << std::dec << std::left;
    switch (_value.index()) {
    case 0:  return os << std::get<0>(_value);
    case 1:  return os << std::get<1>(_value);
    case 2:  return os << static_cast<float>(std::get<2>(_value));
    default: return os;
    }
}

std::ostream &numeric::print_hex(std::ostream &os, size_t size) const
{
    uint8_t data[8] = { 0 };
    for (size_t i = 0, sz = bytes(data, size); i < sz; i++) {
        os << std::hex << std::setfill('0') << std::setw(2) << std::right << static_cast<unsigned>(data[i]);
    }
    return os << "|";
}

std::ostream &numeric::write(std::ostream &os, size_t size) const
{
    uint8_t data[8] = { 0 };
    for (size_t i = 0, sz = bytes(data, size); i < sz; i++) {
        os << data[i];
    }
    return os;
}
