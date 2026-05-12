#pragma once

#include "grid.h"
#include "std_array.h"
#include "core_math.h"

/*
 * A speed-improved perlin and simplex noise algorithms for 2D.
 *
 * Based on example code by Stefan Gustavson (stegu@itn.liu.se).
 * Optimisations by Peter Eastman (peastman@drizzle.stanford.edu).
 * Better rank ordering method by Stefan Gustavson in 2012.
 * Converted to Javascript by Joseph Gentle.
 *
 * Version 2012-03-09
 *
 * This code was placed in the public domain by its original author,
 * Stefan Gustavson. You may use it as you see fit, but
 * attribution is appreciated.
 *
 */

namespace noise {

template <typename FloatT>
struct simplex
{
    typedef std::array<FloatT, 2> vec2_t;
    typedef std::array<FloatT, 3> vec3_t;
    
    typedef std::array<int, 2>    int2_t;
    typedef std::array<int, 3>    int3_t;
    
    simplex(FloatT seed)
    {
        if (seed > 0 && seed < 1) {
            // Scale the seed out
            seed *= 65536;
        }
        uint32_t uint_seed = uint32_t(floor(seed));
        if (uint_seed < 256) {
            uint_seed |= uint_seed << 8;
        }
        for (size_t i = 0; i < 256; i++) {
            uint8_t v = permute[i] ^ ((uint_seed >> ((i & 1) ? 0 : 8)) & 255);
            perm[i] = perm[i + 256] = v;
            grad_p[i] = grad_p[i + 256] = grad3[v % 12];
        }
    }
    
    FloatT evaluate(const vec2_t &p) const
    {
        int2_t i(ints(p + sum(p) * F2));
        vec2_t t(p - i + std_array::sum(i) * G2);

        int2_t j{ 0, 1 };
        j ^= (t[0] > t[1]);

        vec2_t u(t + G2 - j), v(t + G2 * 2 - 1);
        i &= int2_t{ 255, 255 };

        return 70 * (_corner(t, grad_p[i[0] +        perm[i[1]       ]]) +
                     _corner(u, grad_p[i[0] + j[0] + perm[i[1] + j[1]]]) +
                     _corner(v, grad_p[i[0] +    1 + perm[i[1] +    1]]));
    }

    FloatT evaluate(const vec3_t &p) const
    {
        FloatT f3_sump(std_array::sum(p) * F3);
        int3_t i(std_array::ints(p + vec3_t { f3_sump, f3_sump, f3_sump })), j, k;
        FloatT g3_sumi(std_array::sum(i) * G3);
        vec3_t t(p - i + vec3_t { g3_sumi, g3_sumi, g3_sumi });

        FloatT a = t[0], b = t[1], c = t[2];
        if (a >= b) {
            if      (b >= c) { j = corners[4]; k = corners[6]; }
            else if (a >= c) { j = corners[4]; k = corners[5]; }
            else             { j = corners[1]; k = corners[5]; }
        } else {
            if      (b <  c) { j = corners[1]; k = corners[3]; }
            else if (a <  c) { j = corners[2]; k = corners[3]; }
            else             { j = corners[2]; k = corners[6]; }
        }
        
        vec3_t g3_1 { G3 * 1, G3 * 1, G3 * 1 },
               g3_2 { G3 * 2, G3 * 2, G3 * 2 },
               g3_3 { G3 * 3, G3 * 3, G3 * 3 };

        auto u(t - j + G3 * 1), v(t - k + g3_2), w(t - 1 + g3_3);

        i &= int3_t{ 255, 255, 255 };

        return 32. * (_corner(t, grad_p[i[0] +        perm[i[1] +        perm[i[2]       ]]]) +
                      _corner(u, grad_p[i[0] + j[0] + perm[i[1] + j[1] + perm[i[2] + j[2]]]]) +
                      _corner(v, grad_p[i[0] + k[0] + perm[i[1] + k[1] + perm[i[2] + k[2]]]]) +
                      _corner(w, grad_p[i[0] +    1 + perm[i[1] +    1 + perm[i[2] +    1]]]));
    }

private:

    inline static vec2_t _v2(const vec3_t &v) { return vec2_t{ v[0], v[1] }; }
    
    inline static FloatT _corner(const vec3_t &v, const vec3_t &g)
    {
        FloatT t = 0.6 - std_array::dot_self(v), tt = t * t;
        return t < 0 ? 0 : tt * tt * std_array::dot(g, v);
    }

    inline static FloatT _corner(const vec2_t &v, const vec2_t &g)
    {
        FloatT t = 0.5 - std_array::dot_self(v), tt = t * t;
        return t < 0 ? 0 : tt * tt * std_array::dot(g, v);
    }
    
    inline static FloatT _corner(const vec2_t &v, const vec3_t &g) { return _corner(v, rchop(g)); }

    static constexpr const vec3_t grad3[] = {
        vec3_t{ 1., 1., 0. }, vec3_t{ -1.,  1., 0. }, vec3_t{  1., -1.,  0. }, vec3_t{ -1., -1.,  0. },
        vec3_t{ 1., 0., 1. }, vec3_t{ -1.,  0., 1. }, vec3_t{  1.,  0., -1. }, vec3_t{ -1.,  0., -1. },
        vec3_t{ 0., 1., 1. }, vec3_t{  0., -1., 1. }, vec3_t{  0.,  1., -1. }, vec3_t{  0., -1., -1. }
    };

    static constexpr const FloatT SQRT3 = FloatT(math::SQRT3),
                                  F2    = 0.5 * (SQRT3 - 1.0),
                                  G2    = (3.0 - SQRT3) / 6.0,
                                  F3    = 1.0 / 3.0,
                                  G3    = 1.0 / 6.0;

    static constexpr const uint8_t permute[] = {

        151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225, 140,  36, 103,  30,
         69, 142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148, 247, 120, 234,  75,   0,  26, 197,  62,
         94, 252, 219, 203, 117,  35,  11,  32,  57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136,
        171, 168,  68, 175,  74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,
         60, 211, 133, 230, 220, 105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,  65,  25,  63, 161,
          1, 216,  80,  73, 209,  76, 132, 187, 208,  89,  18, 169, 200, 196, 135, 130, 116, 188, 159,  86,
        164, 100, 109, 198, 173, 186,   3,  64,  52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126,
        255,  82,  85, 212, 207, 206,  59, 227,  47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213,
        119, 248, 152,   2,  44, 154, 163,  70, 221, 153, 101, 155, 167,  43, 172,   9, 129,  22,  39, 253,
         19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 112, 104, 218, 246,  97, 228, 251,  34, 242, 193,
        238, 210, 144,  12, 191, 179, 162, 241,  81,  51, 145, 235, 249,  14, 239, 107,  49, 192, 214,  31,
        181, 199, 106, 157, 184,  84, 204, 176, 115, 121,  50,  45, 127,   4, 150, 254, 138, 236, 205,  93,
        222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,  61, 156, 180
    };

    static constexpr const int3_t corners[] = {

        int3_t{ 0, 0, 0 }, int3_t{ 0, 0, 1 }, int3_t{ 0, 1, 0 }, int3_t{ 0, 1, 1 },
        int3_t{ 1, 0, 0 }, int3_t{ 1, 0, 1 }, int3_t{ 1, 1, 0 }, int3_t{ 1, 1, 1 }
    };

    static uint64_t perm[512];
    static vec3_t   grad_p[512];
};

template <typename FloatT> uint64_t              simplex<FloatT>::perm[512];
template <typename FloatT> std::array<FloatT, 3> simplex<FloatT>::grad_p[512];

}
