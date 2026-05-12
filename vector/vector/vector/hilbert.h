#pragma once

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstddef>
#include <bitset>


-- ----


#include "stl.h"

template <size_t Dims, size_t BitsPerDim, typename UnsignedT = uint64_t>
struct hilbert
{
    typedef UnsignedT value_t;
    static constexpr size_t  BITS_PER_DIM = BitsPerDim,
                             DIMENSIONS   = Dims,
                             BITS         = 1 << (_compile_log2_(Dims * BitsPerDim) + 1),
                             BYTES        = (BITS + 7) / 8,
                             WORD_BYTES   = sizeof(UnsignedT),
                             WORD_BITS    = WORD_BYTES * 8,
                             WORDS        = (BYTES + WORD_BYTES - 1) / sizeof(WORD_BYTES);


    typedef std::array<value_t, WORDS> key_t;
    typedef std::array<value_t, Dims>  array_t;

    void key_zcode(const array_t &data, key_t &out)
    {
        out = key_t({ 0 });
        _interleave(data, out);
    }

    void key_hilbert(const array_t &data, key_t &out)
    {
        out = key_t({ 0 });
        _interleave_gray(data, out);
    }

    std::ostream &print_stats(std::ostream &os) const
    {
        return os << "[hilbert Dims:" << Dims << " BitsPerDem:" << BitsPerDim << " total bits:" << BITS << "/bytes:" << BYTES << " | word bits:" << WORD_BITS << "/bytes:" << WORD_BYTES << "/count:" << WORDS << "]";
    }

    void test_bin_to_gray_to_bin()
    {
        size_t mismatches = 0;
        for (size_t i = 0; i != 1024 * 1024; i++) {
            value_t a = _bin_to_gray(i), j = _gray_to_bin(a);
            if (i < 128) std::cout << a << " ";
            if (i != j) mismatches++;
        }
        std::cout << std::endl << "hilbert ";
        if (mismatches > 0) {
            std::cout << "FAILED mismatches: " << mismatches << std::endl;
        } else {
            std::cout << "success!" << std::endl;
        }
    }

    void test_coding()
    {
        array_t in;
        key_t   out;
        std::cout << std::endl << "in  ";
        for (size_t i = 0; i < Dims; ++i) {
            in[i] = i;
            std::cout << in[i] << " ";
        }
        std::cout << std::endl << "key ";
        key_hilbert(in, out);
        for (size_t i = 0; i < WORDS; ++i)
            std::cout << std::bitset<WORD_BITS>(out[i]) << " ";
        std::cout << std::endl;
        for (size_t i = 0; i < WORDS; ++i)
            std::cout << std::hex << i << ":" << out[i] << " ";
        std::cout << std::endl << std::dec;
    }

private:

    static constexpr value_t ONE  = value_t(1);
    static constexpr value_t MASK = (ONE << (BitsPerDim - 1)) | ((ONE << (BitsPerDim - 1)) - 1);

    // TODO test that these are invertible
    inline static value_t _bin_to_gray(value_t n) { return n ^ (n >> 1); }
    inline static value_t _gray_to_bin(value_t n) { value_t b = 0; for (; n; n >>= 1) b ^= n; return b; }

    void _interleave(size_t dimension, value_t n, key_t &out)
    {
        size_t bit = dimension;
        n &= MASK;
        for (size_t i = 0; n; ++i) {
            size_t word = bit / WORD_BITS, wbit = bit - (word * WORD_BITS);
            value_t v(n & 1);
            if (v) out[word] |= v << wbit;
            bit += Dims;
            n >>= 1;
        }
    }

    void _interleave(const array_t &data, key_t &out)
    {
        for (size_t i = 0, sz = data.size(); i != sz; ++i)
            _interleave(i, data[i], out);
    }

    void _interleave_gray(const array_t &data, key_t &out)
    {
        for (size_t i = 0, sz = data.size(); i != sz; ++i)
            _interleave(i, _bin_to_gray(data[i]), out);
    }
};

