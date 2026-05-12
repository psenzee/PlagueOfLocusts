#include "reader.h"
#include "item.h"
#include "agent.h"
#include "collection.h"

reader *reader::_instance = nullptr;

template <typename T> static reader::size_object_t create(binary_in_stream &is, std::size_t bytes = 0)
{
    T *obj = new T;
    bytes += obj->read(is, false);
    return reader::size_object_t(bytes, std::unique_ptr<object>(obj));
}

reader::size_object_t reader::read(binary_in_stream &is) const
{
    object::Type type = object::NONE;
    std::size_t bytes = is.read(type);
    // switch for now, but possible registry later
    switch (type) {
    case object::COLLECTION: return create<collection>(is, bytes);
    case object::ITEM:       return create<item>(is, bytes);
    case object::AGENT:      return create<agent>(is, bytes);
    default:
        break;
    }
    throw std::runtime_error("invalid object type found");
}

reader *reader::instance()
{
    if (!_instance) {
        _instance = new reader;
    }
    return _instance;
}
    
