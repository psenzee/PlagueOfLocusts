#pragma once

#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <array>
#include <vector>

struct binary_stream
{
    virtual ~binary_stream() {}

    virtual void close() = 0;
};

struct binary_in_stream : public binary_stream
{
    virtual std::size_t read(void *buffer, std::size_t bytes)        = 0;
    
    template <typename T> std::size_t read(T &value) { return read(&value, sizeof(value)); }
};

struct binary_out_stream : public binary_stream
{
    virtual std::size_t write(const void *buffer, std::size_t bytes) = 0;
    virtual void        flush()                                      = 0;
    
    template <typename T> std::size_t write(const T &value) { return write(&value, sizeof(value)); }
};
