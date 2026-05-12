#pragma once

#include <array>
#include <cmath>
#include <cstdint>
#include <cstddef>
#include <limits>
#include <algorithm>
#include "primes.h"

template <size_t BitsN>
class bloom
{
    typedef std::array<uint64_t, (BitsN >> 6) + 1> bits_t;

    bits_t _data;
    
    inline bool            _bit(size_t i)       const { i %= BitsN; return (_data[i >> 6] & (1ull << (i & 0x3f))) > 0; }
    inline void            _set(size_t i)             { i %= BitsN; _data[i >> 6] |= 1ull << (i & 0x3f); }
    
    inline static uint64_t _hash_0(uint64_t v)        { return v + 1052133; }
    inline static uint64_t _hash_1(uint64_t v)        { return v; }
    inline static uint64_t _hash_2(uint64_t v)        { return _reverse(uint32_t(v)); }
    
    inline static uint32_t _reverse(uint32_t x)
    {
        x = (x & 0x55555555) <<  1 | ((x >>  1) & 0x55555555);
        x = (x & 0x33333333) <<  2 | ((x >>  2) & 0x33333333);
        x = (x & 0x0f0f0f0f) <<  4 | ((x >>  4) & 0x0f0f0f0f);
        return (x << 24) | ((x & 0xff00) << 8) | ((x >> 8) & 0xff00) | (x >> 24);
    }

public:

    inline bloom() : _data({ 0 }) {}
    
    inline void clear()                     { _data = { 0 }; }
    inline void set(uint64_t v)             { _set(_hash_0(v)); _set(_hash_1(v)); _set(_hash_2(v)); }
    inline bool isset(uint64_t v)     const { return _bit(_hash_0(v)) && _bit(_hash_1(v)) & _bit(_hash_2(v)); }
};

template <size_t BitsN>
void test_bloom(size_t count, size_t offset = 0)
{
    bloom<BitsN> filter;
    for (size_t i = 0; i < count; i++)
        filter.set(i);
    for (size_t i = 0; i < count; i++)
        if (!filter.isset(i))
            // there should never be false negatives
            std::cout << "bloom[" << BitsN << "/" << count << "]: ERROR false negative at " << i << std::endl;
    size_t false_pos_count = 0, test_size = (count * 4);
    for (size_t i = count; i < test_size; i++)
        // some false positives are expected
        if (filter.isset(i))
            false_pos_count++;
    std::cout << "bloom[" << BitsN << "/" << count << "-" << test_size << "]: false positives " << false_pos_count << " rate " << float(false_pos_count * 100) / (test_size - count) << "%" << std::endl;
}
