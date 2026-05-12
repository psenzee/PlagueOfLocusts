#pragma once

#include <cstdint>
#include <random>

namespace {
static constexpr size_t _lg(size_t n) { return n <= 1 ? 0 : (1 + _lg(n >> 1)); }
template <typename U> struct _traits { enum { BITS = sizeof(U) * 8, LG = _lg(BITS), MASK = BITS - 1 }; };
// knuth's from https://matthewarcus.wordpress.com/2012/11/18/reversing-a-64-bit-word/
template <typename T, T M, int K> static inline constexpr T _swap_bits(T p) { T q = ((p >> K) ^ p) & M; return p ^ q ^ (q << K); }
}

inline size_t bit_count(uint64_t v)          { return size_t(__builtin_popcountl(v)); }
inline size_t bit_count(int64_t v)           { return size_t(__builtin_popcountl(v)); }
inline size_t bit_count(uint32_t v)          { return size_t(__builtin_popcount(v)); }
inline size_t bit_count(int32_t v)           { return size_t(__builtin_popcount(v)); }

inline size_t bit_ffs(uint64_t v)            { return size_t(ffsll(v)); }
inline size_t bit_ffs(int64_t v)             { return size_t(ffsll(v)); }
inline size_t bit_ffs(uint32_t v)            { return size_t(ffsl(v)); }
inline size_t bit_ffs(int32_t v)             { return size_t(ffsl(v)); }

// auto-optimized? use intrinsic?
template <typename U> inline U bit_rotl(U x, uint8_t r) { return (x << r) | (x >> (sizeof(U) * 8 - r)); }
template <typename U> inline U bit_rotr(U x, uint8_t r) { return (x >> r) | (x << (sizeof(U) * 8 - r)); }

#if 0 && (defined(__aarch64__) || defined(_M_ARM64))
// don't actually know if this works since I'm currently using x64
inline uint64_t bit_reverse(uint64_t value)
{
    __asm__ ("rbit %0, %1" : "=r" (value) : "r" (value));
    return value;
}
#endif

// knuth's from https://matthewarcus.wordpress.com/2012/11/18/reversing-a-64-bit-word/
inline uint64_t bit_reverse(uint64_t n)
{
  constexpr static const uint64_t
    M0 = 0x5555555555555555ull,
    M1 = 0x0300c0303030c303ull,
    M2 = 0x00c0300c03f0003full,
    M3 = 0x00000ffc00003fffull;
  n = ((n >> 1) & M0) | (n & M0) << 1;
  n = _swap_bits<uint64_t, M1,  4>(n);
  n = _swap_bits<uint64_t, M2,  8>(n);
  n = _swap_bits<uint64_t, M3, 20>(n);
  n = (n >> 34) | (n << 30);
  return n;
}

inline uint64_t bit_reverse__reference(uint64_t n)
{
    constexpr uint64_t M = 1ull << 63;
    uint64_t o = 0, m = M;
    for (size_t i = 0; i < 64; i++) {
        if (n & m) o |= (1ull << i);
        m >>= 1;
    }
    return o;
}

void test_bit_reverse()
{
    std::minstd_rand r;
    for (size_t i = 0; i < 1000; i++) {
        auto n = r();
        uint64_t a = bit_reverse__reference(n), b = bit_reverse(n);
        if (b != a)
            std::cout << "bit reverse failed " << n << " -> " << b << "!=" << a << std::endl;
    }
}
