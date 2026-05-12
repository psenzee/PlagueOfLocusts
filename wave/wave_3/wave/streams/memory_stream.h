#pragma once

#include <string>
#include <cstdio>
#include "binary_stream.h"

struct memory_in_stream : public binary_in_stream
{
    memory_in_stream(const void *buffer, std::size_t size);
    
    std::size_t read(void *u, std::size_t bytes);
    void        close() {}
    
private:

    const uint8_t *_buffer;
    std::size_t    _size;
    const uint8_t *_ptr;
};

struct memory_out_stream : public binary_out_stream
{
    memory_out_stream(void *buffer, std::size_t size) : _buffer((uint8_t *)buffer), _size(size), _ptr((uint8_t *)buffer) {}

    std::size_t write(const void *u, std::size_t bytes);
    void        close() {}
    void        flush() {}

private:

    uint8_t     *_buffer;
    std::size_t  _size;
    uint8_t     *_ptr;
};
