#pragma once

#include "binary_stream.h"

#include <cstdint>
#include <cstdio>
#include <string>
#include <string_view>
#include <array>
#include <vector>

#include "format.h"

struct serializable {};

template <typename SizeT> std::size_t write_sized_string(binary_out_stream &s, std::string_view message)
{
    SizeT size(message.size());
    std::size_t bytes = s.write(size);
    return bytes + s.write(message.data(), message.size());
}

template <typename SizeT> std::size_t read_sized_string(binary_in_stream &s, std::string &message)
{
    SizeT size = 0;
    std::size_t bytes = s.read(size);
    message.resize(size);
    return bytes + s.read(message.data(), size);;
}

template <typename UIntT>
void write_variable_length_uint(binary_out_stream &s, UIntT value)
{
    /*
    Accept an integer, and serialize it as a MIDI file variable length quantity

    Some numbers in MTrk chunks are represented in a form called a variable-
    length quantity.  These numbers are represented in a sequence of bytes,
    each byte holding seven bits of the number, and ordered most significant
    bits first. All bytes in the sequence except the last have bit 7 set,
    and the last byte has bit 7 clear.  This form allows smaller numbers to
    be stored in fewer bytes.  For example, if the number is between 0 and
    127, it is thus represented exactly as one byte.  A number between 128
    and 16383 uses two bytes, and so on.

    Examples:
    Number  VLQ
    128     81 00
    8192    C0 00
    16383   FF 7F
    16384   81 80 00
    */
    
    uint8_t bytes[sizeof(UIntT) * 2] = { 0 };
    
    if (value == 0) {
        s.write(uint8_t(0));
        return;
    }

    // low-order byte has high bit cleared.
    uint8_t hi = 0;
    int i = 0;
    while (value > 0) {
        bytes[i] = uint8_t(((value & 0x7f) | hi) & 0xff);
        value >>= 7;
        hi = 0x80;
        i++;
    }

    // put most-significant byte first, least significant last
    while (i) {
        s.write(bytes[--i]);
    }
}


std::size_t write_format(binary_out_stream &s, std::string_view fmt, ...);

template <typename SizeT>
std::size_t write_format_sized(binary_out_stream &s, std::string_view fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    std::string formatted = format::format_args(fmt.data(), args);
    va_end(args);
    return write_sized_string<SizeT>(formatted, formatted.size()) == formatted.size();
}

template <typename SerializeT> std::size_t data_write(binary_out_stream &s, const SerializeT &u, std::enable_if_t<std::is_base_of<serializable, SerializeT>::value, int> = 0)
{
    return u.data_write(s);
}

template <typename SerializeT> std::size_t data_write(binary_out_stream &s, const SerializeT &u, std::enable_if_t<!std::is_base_of<serializable, SerializeT>::value, int> = 0)
{
    return s.write(u);
}

template <typename SerializeT> std::size_t data_read(binary_in_stream &s, SerializeT &u, std::enable_if_t<std::is_base_of<serializable, SerializeT>::value, int> = 0)
{
    return u.data_read(s);
}

template <typename SerializeT> std::size_t data_read(binary_in_stream &s, SerializeT &u, std::enable_if_t<!std::is_base_of<serializable, SerializeT>::value, int> = 0)
{
    return s.read(u);
}

template <typename SerializeT> std::size_t data_write(binary_out_stream &s, const std::vector<SerializeT> &vec)
{
    std::size_t bytes = s.write(vec.size());
    for (const auto &u : vec) {
        bytes += data_write(s, u);
    }
    return bytes;
}

template <typename SerializeT> std::size_t data_write(binary_out_stream &s, const std::vector<SerializeT *> &vec)
{
    std::size_t bytes = s.write(vec.size());
    for (const auto *u : vec) {
        bytes += data_write(s, *u);
    }
    return bytes;
}

template <typename SerializeT> std::size_t data_write(binary_out_stream &s, const std::vector< std::unique_ptr<SerializeT> > &vec)
{
    std::size_t bytes = s.write(vec.size());
    for (const auto &u : vec) {
        bytes += data_write(s, *u);
    }
    return bytes;
}

template <typename SerializeT, std::size_t N> std::size_t data_write(binary_out_stream &s, const std::array<SerializeT, N> &arr)
{
    std::size_t bytes = s.write(arr.size());
    for (const auto &u : arr) {
        bytes += data_write(s, u);
    }
    return bytes;
}

template <typename SerializeT, std::size_t N> std::size_t data_write(binary_out_stream &s, const std::array<SerializeT *, N> &arr)
{
    std::size_t bytes = s.write(arr.size());
    for (const auto *u : arr) {
        bytes += data_write(s, *u);
    }
    return bytes;
}
///--
template <typename T, typename U> std::size_t data_read(binary_in_stream &s, std::unordered_map<T, U> &map)
{
    map.clear();
    std::size_t size = 0, bytes = s.read(size);
    U u;
    T t;
    for (std::size_t i = 0; i < size; ++i) {
        bytes += data_read(s, t) + data_read(s, u);
        map[t] = u;
    }
    return bytes;
}

template <typename T, typename U> std::size_t data_read(binary_in_stream &s, std::unordered_map<T, U *> &map)
{
    map.clear();
    std::size_t size = 0, bytes = s.read(size);
    U *u = nullptr;
    T t;
    for (std::size_t i = 0; i < size; ++i) {
        u = new U;
        bytes += data_read(s, t) + data_read(s, *u);
        map[t] = u;
    }
    return bytes;
}
///----
template <typename SerializeT> std::size_t data_read(binary_in_stream &s, std::vector<SerializeT> &vec)
{
    vec.clear();
    std::size_t size = 0, bytes = s.read(size);
    SerializeT u;
    for (std::size_t i = 0; i < size; ++i) {
        bytes += data_read(s, u);
        vec.push_back(u);
    }
    return bytes;
}

template <typename SerializeT> std::size_t data_read(binary_in_stream &s, std::vector<SerializeT *> &vec)
{
    vec.clear();
    std::size_t size = 0, bytes = s.read(size);
    SerializeT *u = nullptr;
    for (std::size_t i = 0; i < size; ++i) {
        u = new SerializeT;
        bytes += data_read(s, *u);
        vec.push_back(u);
    }
    return bytes;
}


template <typename SerializeT> std::size_t data_read(binary_in_stream &s, std::vector< std::unique_ptr<SerializeT> > &vec)
{
    vec.clear();
    std::size_t size = 0, bytes = s.read(size);
    for (std::size_t i = 0; i < size; ++i) {
        vec.emplace_back(std::make_unique<SerializeT>());
        bytes += data_read(s, *vec.back());
    }
    return bytes;
}

template <typename SerializeT, std::size_t N> std::size_t data_read(binary_in_stream &s, std::array<SerializeT, N> &ar)
{
    std::size_t size = 0, bytes = s.read(size);
    for (std::size_t i = 0; i < size; ++i) {
        bytes += data_read(s, ar[i]);
    }
    for (std::size_t i = size; i < N; ++i) {
        ar[i] = SerializeT();
    }
    return bytes;
}

template <typename SerializeT, std::size_t N> std::size_t data_read(binary_in_stream &s, std::array<SerializeT *, N> &ar)
{
    std::size_t size = 0, bytes = s.read(size);
    for (std::size_t i = 0; i < size; ++i) {
        auto *u = new SerializeT;
        bytes += data_read(s, *u);
        ar[i] = u;
    }
    for (std::size_t i = size; i < N; ++i) {
        ar[i] = nullptr;
    }
    return bytes;
}

template <typename SerializeT, std::size_t N> std::size_t data_read(binary_in_stream &s, std::array<std::unique_ptr<SerializeT>, N> &vec)
{
    vec.clear();
    std::size_t size = 0, bytes = s.read(size);
    for (std::size_t i = size; i < N; ++i) {
        vec.emplace_back(std::make_unique<SerializeT>());
        bytes += data_read(s, *vec.back());
    }
    return bytes;
}
