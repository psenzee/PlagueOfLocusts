#include "region_info.h"

std::size_t region_info::data_write(binary_out_stream &os) const
{
    return os.write(_region) + _info.data_write(os);
}

std::size_t region_info::data_read(binary_in_stream &is)
{
    std::size_t bytes = is.read(_region);
    return bytes + _info.data_read(is);
}
