#pragma once

#include <cstddef>

// --- 64 bits ---

// 8 bits into 64
template <typename T0,           typename T1 = uint8_t, typename T2 = uint8_t, typename T3 = uint8_t,
          typename T4 = uint8_t, typename T5 = uint8_t, typename T6 = uint8_t, typename T7 = uint8_t>
inline uint64_t pack_bits_64_from_8s(T0 a, T1 b = 0, T2 c = 0, T3 d = 0, T4 e = 0, T5 f = 0, T6 g = 0, T7 h = 0)
{
    typedef uint64_t u;
    enum { M = 0x0ff };
    return (u(a & M) << 56) | (u(b & M) << 48) | (u(c & M) << 40) | (u(d & M) << 32) |
           (u(e & M) << 24) | (u(f & M) << 16) | (u(g & M) <<  8) |  u(h & M);
}

// 16 bits into 64
template <typename T0, typename T1 = uint16_t, typename T2 = uint16_t, typename T3 = uint16_t>
inline uint64_t pack_bits_64_from_16s(T0 a, T1 b = 0, T2 c = 0, T3 d = 0)
{
    typedef uint64_t u;
    enum { M = 0x0ffff };
    return (u(a & M) << 48) | (u(b & M) << 32) | (u(c & M) << 16) | u(d & M);
}

// 32 bits into 64
template <typename T0, typename T1 = uint32_t>
inline uint64_t pack_bits_64_from_32s(T0 a, T1 b = 0)
{
    typedef uint64_t u;
    enum { M = 0x0ffffffffull };
    return (u(a & M) << 32) | u(b & M);
}

// --- 32 bits ---

// 8 bits into 32
template <typename T0, typename T1 = uint8_t, typename T2 = uint8_t, typename T3 = uint8_t>
inline uint32_t pack_bits_32_from_8s(T0 a, T1 b = 0, T2 c = 0, T3 d = 0)
{
    typedef uint32_t u;
    enum { M = 0x0ff };
    return (u(a & M) << 24) | (u(b & M) << 16) | (u(c & M) << 8) | u(d & M);
}

// 16 bits into 32
template <typename T0, typename T1 = uint16_t>
inline uint32_t pack_bits_32_from_16s(T0 a, T1 b = 0)
{
    typedef uint32_t u;
    enum { M = 0x0ffff };
    return (u(a & M) << 16) | u(b & M);
}

// --- 16 bits ---

// 8 bits into 16
template <typename T0, typename T1 = uint8_t>
inline uint16_t pack_bits_16_from_8s(T0 a, T1 b = 0)
{
    typedef uint16_t u;
    enum { M = 0x0ff };
    return (u(a & M) << 8) | u(b & M);
}
