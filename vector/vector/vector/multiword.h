#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>
#include <array>

constexpr size_t _compile_log2_(size_t n) { return ((n < 2) ? 1 : (1 + _compile_log2_(n / 2))); }

template <size_t WordsN, typename UIntT = uint64_t>
struct multiword
{
    typedef multiword<WordsN, UIntT>  self_t;
    typedef UIntT                     word_t;
    typedef std::array<UIntT, WordsN> data_t;
    
    static constexpr size_t COUNT         = WordsN,
                            WORD_BITS     = sizeof(word_t) * 8,
                            LOG_WORD_BITS = _compile_log2_(WORD_BITS),
                            TOTAL_BITS    = WORD_BITS * COUNT;

    data_t _data;

    inline multiword(const data_t &u) : _data(u.data) {}
    inline multiword() : _data({ 0 }) {}

    self_t &_add(const data_t &b)
    {
        word_t carry = 0;
        for (size_t i = 0; i < COUNT; ++i) {
            word_t t = _data[i] + carry, b = b._data[i];
            // If overflow occurred, t will be less than carry
            carry = (t < carry);
            t += b;
            // If overflow occurred, t will be less than b
            carry += (t < b);
            _data[i] = t;
        }
        return *this;
    }

    self_t &_shift_left(size_t bits)
    {
        if (bits >= WORD_BITS) {
            // Shift by whole words
            size_t word_shift = bits >> LOG_WORD_BITS;
            for (size_t i = COUNT - 1; i >= word_shift; --i)
                _data[i] = _data[i - word_shift];
            // Zero out the remaining words
            for (size_t i = 0; i < word_shift; ++i)
                _data[i] = 0;
            // Adjust bits for remaining shift
            bits &= (WORD_BITS - 1);
        }
        if (bits > 0) {
            // Shift by remaining bits
            for (size_t i = COUNT - 1; i > 0; --i)
                _data[i] = (_data[i] << bits) | (_data[i - 1] >> (WORD_BITS - bits));
            _data[0] <<= bits;
        }
        return *this;
    }

    self_t &_shift_right(size_t bits)
    {
        if (bits >= WORD_BITS) {
            // Shift by whole words
            size_t word_shift = bits >> LOG_WORD_BITS;
            for (size_t i = 0; i < COUNT - word_shift; ++i)
                _data[i] = _data[i + word_shift];
            // Zero out the remaining words
            for (size_t i = COUNT - word_shift; i < N; ++i)
                _data[i] = 0;
            // Adjust bits for remaining shift
            bits &= (WORD_BITS - 1);
        }
        if (bits > 0) {
            // Shift by remaining bits
            for (size_t i = 0; i < Words - 1; ++i)
                _data[i] = (_data[i] >> bits) | (_data[i + 1] << (WORD_BITS - bits));
            _data[COUNT - 1] >>= bits;
        }
        return *this;
    }
}


/*
 using namespace std;

 // Helper functions
 unsigned int bit_to_transpose(unsigned int bit, unsigned int dimensions){
     unsigned int t = bit;
     unsigned int s = 1;
     while(s < dimensions){
         t = t ^ (t >> s);
         s *= 2;
     }
     return t;
 }

 unsigned int transpose_to_bit(unsigned int transposed, unsigned int direction, unsigned int dimensions){
     unsigned int t = transposed;
     unsigned int s = dimensions;
     while (s > 1){
         s /= 2;
         t = t ^ (t >> s);
     }
     return (((t << direction) | (t >> (dimensions-direction))) & ((1 << dimensions)-1));
 }

 vector<unsigned int> point_add(vector<unsigned int> point, unsigned int direction, unsigned int offset, unsigned int dimensions){
     vector<unsigned int> result(dimensions);
     for(unsigned int i = 0; i < dimensions; i++){
         result[i] = (point[i] + ((offset >> ((direction+i)%dimensions)) & 1)) & ((1 << dimensions) - 1);
     }
     return result;
 }

 // Main functions
 unsigned int point_to_hilbert_index(vector<unsigned int> point, unsigned int dimensions, unsigned int order){
     unsigned int bits = 0;
     unsigned int direction = 0;
     for (int i = order-1; i >= 0; i--){
         unsigned int transposed = transpose_to_bit(point[direction], direction, dimensions);
         bits = (bits << dimensions) + transposed;
         point = point_add(point, direction, transposed, dimensions);
         direction = (direction + transposed + 1) % dimensions;
     }
     return bits;
 }
*/

template <typename UIntT, size_t Dim>
inline UIntT _bit_to_transpose(UIntT bit)
{
    UIntT t = bit, s = 1;
    while (s < Dim) { t ^= (t >> s); s <<= 1; }
    return t;
}

template <typename UIntT, size_t Dim>
UIntT _transpose_to_bit(UIntT transposed, UIntT direction)
{
    UIntT t = transposed, s = Dim, dir = direction;
    while (s > 1) { s >>= 1; t ^= (t >> s); }
    return (((t << dir) | (t >> (Dim - dir))) & ((UIntT(1) << Dim) - 1));
}

template <typename UIntT, size_t Dim>
std::array<UIntT, Dim> _point_add(const std::array<UIntT, Dim> &point, UIntT direction, UIntT offset)
{
    std::array<UIntT, Dim> r({ 0 });
    for(size_t i = 0; i < Dim; i++) {
        r[i] = (point[i] + ((offset >> ((direction + i) & (Dim - 1))) & 1)) & ((UIntT(1) << Dim) - 1);
    }
    return r;
}

// Main functions
template <typename UIntT, size_t Dim>
UIntT point_to_hilbert_index(const std::array<UIntT, Dim> &point, UIntT order)
{
    UIntT bits = 0, direction = 0;
    for (int i = order-1; i >= 0; i--){
        UIntT transposed = _transpose_to_bit(point[direction], direction, Dim);
        bits = (bits << Dim) + transposed;
        point = _point_add(point, direction, transposed, Dim);
        direction = (direction + transposed + 1) & (Dim - 1);
    }
    return bits;
}
/*
template <typename UIntT, size_t Words>
void mw_add(std::array<UIntT, Words> &a, const std::array<UIntT, Words> &b)
{
    UIntT carry = 0;
    for (size_t i = 0; i < Words; ++i) {
        uint64_t temp = a[i] + carry;
        carry = (temp < carry);  // If overflow occurred, temp will be less than carry
        temp += b[i];
        carry += (temp < b[i]);  // If overflow occurred, temp will be less than b[i]
        a[i] = temp;
    }
}

template <typename UIntT, size_t Words>
void mw_shift_left(std::array<UIntT, Words> &value, size_t bits)
{
    static constexpr UIntT BITS = sizeof(UIntT) * 8;
    if (bits >= BITS) {
        // Shift by whole words
        size_t word_shift = bits / BITS;
        for (size_t i = Words - 1; i >= word_shift; --i) {
            value[i] = value[i - word_shift];
        }
        // Zero out the remaining words
        for (size_t i = 0; i < word_shift; ++i) {
            value[i] = 0;
        }
        // Adjust bits for remaining shift
        bits &= (BITS - 1);
    }
    if (bits > 0) {
        // Shift by remaining bits
        for (size_t i = Words - 1; i > 0; --i) {
            value[i] = (value[i] << bits) | (value[i - 1] >> (BITS - bits));
        }
        value[0] <<= bits;
    }
}

template <typename UIntT, size_t Words>
void mw_shift_right(std::array<UIntT, Words> &value, size_t bits)
{
    static constexpr UIntT BITS = sizeof(UIntT) * 8;
    if (bits >= BITS) {
        // Shift by whole words
        size_t word_shift = bits >> _compile_log2_(BITS);
        for (size_t i = 0; i < Words - word_shift; ++i) {
            value[i] = value[i + word_shift];
        }
        // Zero out the remaining words
        for (size_t i = Words - word_shift; i < N; ++i) {
            value[i] = 0;
        }
        // Adjust bits for remaining shift
        bits &= (BITS - 1);
    }
    if (bits > 0) {
        // Shift by remaining bits
        for (size_t i = 0; i < Words - 1; ++i) {
            value[i] = (value[i] >> bits) | (value[i + 1] << (BITS - bits));
        }
        value[Word - 1] >>= bits;
    }
}
*/
// ----

// tests
/*

#include "multiword.h"
#include <gtest/gtest.h>
#include <random>

// Tests the _add() function
TEST(MultiwordTest, AddTest) {
    multiword<4> a({1, 2, 3, 4});
    multiword<4> b({4, 3, 2, 1});
    a._add(b._data);
    EXPECT_EQ(a._data, (std::array<uint64_t, 4>{5, 5, 5, 5}));
}

// Tests the _shift_left() function
TEST(MultiwordTest, ShiftLeftTest) {
    multiword<4> a({1, 2, 3, 4});
    a._shift_left(2);
    EXPECT_EQ(a._data, (std::array<uint64_t, 4>{4, 8, 12, 16}));
}

// Tests the _shift_right() function
TEST(MultiwordTest, ShiftRightTest) {
    multiword<4> a({8, 16, 24, 32});
    a._shift_right(2);
    EXPECT_EQ(a._data, (std::array<uint64_t, 4>{2, 4, 6, 8}));
}

// Tests the random values
TEST(MultiwordTest, RandomTest) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(1, 10);

    for (int i = 0; i < 100; ++i) {
        int value = distrib(gen);

        multiword<4> a({value, value, value, value});
        multiword<4> b({value, value, value, value});

        a._add(b._data);
        for (auto v : a._data) {
            EXPECT_EQ(v, 2 * value);
        }

        a._shift_left(1);
        for (auto v : a._data) {
            EXPECT_EQ(v, 4 * value);
        }

        a._shift_right(1);
        for (auto v : a._data) {
            EXPECT_EQ(v, 2 * value);
        }
    }
}
*/
