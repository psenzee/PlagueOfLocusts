#pragma once

#include "grid.h"
#include "std_array.h"
#include "functors.h"

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

namespace {

typedef std::array<double, 2> vec2_t;
typedef std::array<double, 3> vec3_t;
typedef std::array<double, 4> vec4_t;
typedef std::array<int, 2>    int2_t;
typedef std::array<int, 3>    int3_t;

}

namespace noise {

struct simplex
{
    simplex(double seed)
    {
        if (seed > 0.0 && seed < 1.0) {
            // Scale the seed out
            seed *= 65536.0;
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
    
    double evaluate(const vec2_t &p) const
    {
        double xin = p[0], yin = p[1];   // Noise contributions from the three corners
        // Skew the input space to determine which simplex cell we're in
        double s = (xin + yin) * F2; // Hairy factor for 2D
        int i = int(xin + s), j = int(yin + s);
        double t = (i + j) * G2;
        double x0 = xin - i + t; // The x,y distances from the cell origin, unskewed.
        double y0 = yin - j + t;
        // For the 2D case, the simplex shape is an equilateral triangle.
        // Determine which simplex we are in.
        int i1 = 0, j1 = 1; // Offsets for second (middle) corner of simplex in (i,j) coords
        // upper triangle, YX order: (0,0)->(0,1)->(1,1)
        if (x0 > y0) { // lower triangle, XY order: (0,0)->(1,0)->(1,1)
          i1 ^= 1; j1 ^= 1;
        }
        // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
        // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
        // c = (3-sqrt(3))/6
        double x1 = x0 - i1 + G2; // Offsets for middle corner in (x,y) unskewed coords
        double y1 = y0 - j1 + G2;
        double x2 = x0 - 1 + 2 * G2; // Offsets for last corner in (x,y) unskewed coords
        double y2 = y0 - 1 + 2 * G2;
        // Work out the hashed gradient indices of the three simplex corners
        i &= 255;
        j &= 255;

        // Calculate the contribution from the three corners

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        return 70 * (_corner(vec2_t({ x0, y0 }), _v2(grad_p[i +      perm[j     ]])) +
                     _corner(vec2_t({ x1, y1 }), _v2(grad_p[i + i1 + perm[j + j1]])) +
                     _corner(vec2_t({ x2, y2 }), _v2(grad_p[i +  1 + perm[j +  1]])));
    }

    double evaluate(const vec3_t &p) const
    {
        using namespace std_array;

        // Skew the input space to determine which simplex cell we're in
        vec3_t vi = floor(p + ::sum(p) * F3); // F3 Hairy factor for 3D
        double t = sum(vi) * G3;
        vec3_t v(p - vi + t);
        auto i0(cast<double, int>(vi)); // Offsets for first corner of simplex in (i,j,k) coords
        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int3_t i1, i2;   // Offsets for third corner of simplex in (i,j,k) coords
        if (v[0] >= v[1]) {
            if      (v[1] >= v[2]) { i1 = corners[4]; i2 = corners[6]; }
            else if (v[0] >= v[2]) { i1 = corners[4]; i2 = corners[5]; }
            else                   { i1 = corners[1]; i2 = corners[5]; }
        } else {
            if      (v[1] <  v[2]) { i1 = corners[1]; i2 = corners[3]; }
            else if (v[0] <  v[2]) { i1 = corners[2]; i2 = corners[3]; }
            else                   { i1 = corners[2]; i2 = corners[6]; }
        }
        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        // c = 1/6.

        auto v1(v - i1 + G3 * 1);  // Offsets for second corner
        auto v2(v - i2 + G3 * 2);  // Offsets for third corner
        auto v3(v - 1. + G3 * 3);  // Offsets for fourth corner

        // Work out the hashed gradient indices of the four simplex corners
        i0 &= 255;

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].

        return 32. * (_corner(v,  grad_p[i0[0] +         perm[i0[1] +         perm[i0[2]        ]]]) +
                      _corner(v1, grad_p[i0[0] + i1[0] + perm[i0[1] + i1[1] + perm[i0[2] + i1[2]]]]) +
                      _corner(v2, grad_p[i0[0] + i2[0] + perm[i0[1] + i2[1] + perm[i0[2] + i2[2]]]]) +
                      _corner(v3, grad_p[i0[0] +     1 + perm[i0[1] +     1 + perm[i0[2] +     1]]]));
    }

private:

    inline static vec2_t _v2(const vec3_t &v) { return vec2_t{ v[0], v[1] }; }
    
    inline static double _corner(const vec3_t &v, const vec3_t &g)
    {
        double t = 0.6 - dot_self(v), tt = t * t;
        return t < 0 ? 0 : tt * tt * dot(g, v);
    }

    inline static double _corner(const vec2_t &v, const vec2_t &g)
    {
        double t = 0.5 - dot_self(v), tt = t * t;
        return t < 0 ? 0 : tt * tt * dot(g, v);
    }

    static constexpr const vec3_t grad3[] = {

        vec3_t{ 1., 1., 0. }, vec3_t{ -1.,  1., 0. }, vec3_t{  1., -1.,  0. }, vec3_t{ -1., -1.,  0. },
        vec3_t{ 1., 0., 1. }, vec3_t{ -1.,  0., 1. }, vec3_t{  1.,  0., -1. }, vec3_t{ -1.,  0., -1. },
        vec3_t{ 0., 1., 1. }, vec3_t{  0., -1., 1. }, vec3_t{  0.,  1., -1. }, vec3_t{  0., -1., -1. }
    };

    static constexpr const double SQRT3 = 1.73205080757,
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

uint64_t simplex::perm[512];
vec3_t   simplex::grad_p[512];

struct billow
{
    inline billow(double seed) : _simplex(seed) {}
    inline double evaluate(const vec2_t &p) const { return std::abs(_simplex.evaluate(p)); }
    inline double evaluate(const vec3_t &p) const { return std::abs(_simplex.evaluate(p)); }
private:
    simplex _simplex;
};

struct ridge
{
    inline ridge(double seed) : _simplex(seed) {}
    inline double evaluate(const vec2_t &p) const { return 1.0 - std::abs(_simplex.evaluate(p)); }
    inline double evaluate(const vec3_t &p) const { return 1.0 - std::abs(_simplex.evaluate(p)); }
private:
    simplex _simplex;
};

}
