#include "place_info.h"
#include "serialize.h"
#include <iostream>

std::size_t place_info::data_write(binary_out_stream &os) const
{
    std::size_t bytes = write_sized_string<uint16_t>(os, _name);
    std::cout << "place_info bytes written " << bytes << "\n";
    return bytes;
}

std::size_t place_info::data_read(binary_in_stream &is)
{
    std::size_t bytes = read_sized_string<uint16_t>(is, _name);
    std::cout << "place_info bytes read " << bytes << "\n";
    return bytes;
}
