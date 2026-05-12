#include "memory_stream.h"

#include <cstdio>
#include <cstring>
    
std::size_t memory_in_stream::read(void *u, std::size_t bytes)
{
    std::size_t stream_size = _ptr - _buffer;
    if (stream_size < _size) {
        if (stream_size + bytes >= _size) {
            bytes = _size - stream_size;
        }
        memcpy(u, _ptr, bytes);
        _ptr += bytes;
        return bytes;
    }
    return 0;
}

std::size_t memory_out_stream::write(const void *u, std::size_t bytes)
{
    std::size_t stream_size = _ptr - _buffer;
    if (stream_size < _size) {
        if (stream_size + bytes >= _size) {
            bytes = _size - stream_size;
        }
        memcpy(_ptr, u, bytes);
        _ptr += bytes;
        return bytes;
    }
    return 0;
}
