#include "regions_info.h"
#include <iostream>

regions_info::regions_iter_t regions_info::find(const point2i &p, regions_iter_t begin) const
{
    if (!_region.contains(p)) {
        return _regions.end();
    }
    for (auto i = begin; i != _regions.end(); ++i) {
        if (i->get_region().contains(p)) {
            return i;
        }
    }
    return _regions.end();
}

std::size_t regions_info::data_write(binary_out_stream &os) const
{
    std::size_t bytes = os.write(_region);
    bytes += os.write(_regions.size());
    for (const auto &r : _regions) {
        bytes += r.data_write(os);
    }
    std::cout << "regions_info bytes written " << bytes << "\n";
    return bytes;
}

std::size_t regions_info::data_read(binary_in_stream &is)
{
    std::size_t bytes = is.read(_region), size;
    bytes += is.read(size);
    _regions.clear();
    region_info ri;
    for (std::size_t i = 0; i < size; ++i) {
        bytes += ri.data_read(is);
        _regions.push_back(ri);
    }
    std::cout << "regions_info bytes read " << bytes << "\n";
    return bytes;
}
