#include "collection.h"
#include "reader.h"

std::size_t collection::write(binary_out_stream &os) const
{
    std::size_t bytes = os.write(type());
    bytes += os.write(_objects.size());
    for (const auto &obj : _objects) {
        bytes += ::data_write(os, obj);
    }
    return bytes;
}

std::size_t collection::read(binary_in_stream &is, bool read_type)
{
    std::size_t bytes = 0, size = 0;
    if (read_type) {
        Type this_type = NONE;
        bytes += is.read(this_type);
        if (this_type != type()) {
            throw std::runtime_error("expected collection type");
        }
    }
    bytes += is.read(size);
    _objects.clear();
    for (std::size_t i = 0; i < size; i++) {
        auto size_obj = reader::instance()->read(is);
        bytes += size_obj.first;
        add(std::move(size_obj.second));
    }
    return bytes;
}
