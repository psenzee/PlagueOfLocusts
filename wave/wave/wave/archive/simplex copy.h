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

inline double dot(const vec3_t &v, double x, double y, double z) { return v[0] * x + v[1] * y + v[2] * z; }
inline double dot(const vec3_t &v, double x, double y)           { return v[0] * x + v[1] * y; }
inline double dot(const vec2_t &v, double x, double y)           { return v[0] * x + v[1] * y; }

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
        double xin = p[0], yin = p[1],
               n0 = 0., n1 = 0., n2 = 0.;   // Noise contributions from the three corners
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
        const vec3_t &gi0 = grad_p[i + perm[j]];
        const vec3_t &gi1 = grad_p[i + i1 + perm[j + j1]];
        const vec3_t &gi2 = grad_p[i + 1 + perm[j + 1]];
        // Calculate the contribution from the three corners
        double t0 = 0.5 - x0 * x0 - y0 * y0;
        
        if (t0 < 0) {
            n0 = 0;
        } else {
            t0 *= t0;
            n0 = t0 * t0 * dot(gi0, x0, y0);  // (x,y) of grad3 used for 2D gradient
        }
        double t1 = 0.5 - x1 * x1 - y1 * y1;
        if (t1 < 0) {
            n1 = 0;
        } else {
            t1 *= t1;
            n1 = t1 * t1 * dot(gi1, x1, y1);
        }
        double t2 = 0.5 - x2 * x2 - y2 * y2;
        if (t2 < 0) {
            n2 = 0;
        } else {
            t2 *= t2;
            n2 = t2 * t2 * dot(gi2, x2, y2);
        }
        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        return 70 * (n0 + n1 + n2);
    }
    /*
    double evaluate(const vec3_t &p) const
    {
        double xin = p[0], yin = p[1], zin = p[2],
               n0 = 0., n1 = 0., n2 = 0., n3 = 0.;   // Noise contributions from the three corners
        // Skew the input space to determine which simplex cell we're in
        double s = (xin + yin + zin) * F3; // Hairy factor for 3D
        int i = int(xin + s), j = int(yin + s), k = int(zin + s);
        double t = (i + j + k) * G3;
        double x0 = xin - i + t; // The x,y distances from the cell origin, unskewed.
        double y0 = yin - j + t;
        double z0 = zin - k + t;

        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
        int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords
        if (x0 >= y0) {
            if (y0 >= z0)      { i1=1; j1=0; k1=0; i2=1; j2=1; k2=0; }
            else if (x0 >= z0) { i1=1; j1=0; k1=0; i2=1; j2=0; k2=1; }
            else               { i1=0; j1=0; k1=1; i2=1; j2=0; k2=1; }
        } else {
            if (y0 < z0)       { i1=0; j1=0; k1=1; i2=0; j2=1; k2=1; }
            else if (x0 < z0)  { i1=0; j1=1; k1=0; i2=0; j2=1; k2=1; }
            else               { i1=0; j1=1; k1=0; i2=1; j2=1; k2=0; }
        }
        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        // c = 1/6.

        double x1 = x0 - i1 + G3; // Offsets for second corner
        double y1 = y0 - j1 + G3;
        double z1 = z0 - k1 + G3;

        double x2 = x0 - i2 + 2 * G3; // Offsets for third corner
        double y2 = y0 - j2 + 2 * G3;
        double z2 = z0 - k2 + 2 * G3;

        double x3 = x0 - 1 + 3 * G3; // Offsets for fourth corner
        double y3 = y0 - 1 + 3 * G3;
        double z3 = z0 - 1 + 3 * G3;

        // Work out the hashed gradient indices of the four simplex corners
        i &= 255;
        j &= 255;
        k &= 255;

        const vec3_t &gi0 = grad_p[i +      perm[j +      perm[k     ]]];
        const vec3_t &gi1 = grad_p[i + i1 + perm[j + j1 + perm[k + k1]]];
        const vec3_t &gi2 = grad_p[i + i2 + perm[j + j2 + perm[k + k2]]];
        const vec3_t &gi3 = grad_p[i +  1 + perm[j +  1 + perm[k +  1]]];
        
        // Calculate the contribution from the four corners
        double t0 = 0.6 - x0*x0 - y0*y0 - z0*z0;
        if (t0<0) {
          n0 = 0;
        } else {
          t0 *= t0;
          n0 = t0 * t0 * dot(gi0, x0, y0, z0);  // (x,y) of grad3 used for 2D gradient
        }
        double t1 = 0.6 - x1*x1 - y1*y1 - z1*z1;
        if (t1 < 0) {
          n1 = 0;
        } else {
            t1 *= t1;
            n1 = t1 * t1 * dot(gi1, x1, y1, z1);
        }
        double t2 = 0.6 - x2*x2 - y2*y2 - z2*z2;
        if (t2 < 0) {
            n2 = 0;
        } else {
            t2 *= t2;
            n2 = t2 * t2 * dot(gi2, x2, y2, z2);
        }
        double t3 = 0.6 - x3*x3 - y3*y3 - z3*z3;
        if (t3 < 0) {
            n3 = 0;
        } else {
            t3 *= t3;
            n3 = t3 * t3 * dot(gi3, x3, y3, z3);
        }
        
        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        return 32 * (n0 + n1 + n2 + n3);
    }
     */
    
    inline static double _corner(const vec3_t &v, const vec3_t &g)
    {
        double t = 0.6 - dot_self(v);
        if (t < 0) return 0;
        t *= t;
        return t * t * dot(g, v);
    }
    
    double evaluate2(const vec3_t &p) const
    {
        using namespace std_array;

        // Skew the input space to determine which simplex cell we're in
        vec3_t vi0 = floor(p + ::sum(p) * F3); // F3 Hairy factor for 3D
        double t = sum(vi0) * G3;
        vec3_t v0(p - vi0 + t);
        auto i0(cast<double, int>(vi0)); // Offsets for first corner of simplex in (i,j,k) coords
        const int3_t i000({ 0, 0, 0 }), i001({ 0, 0, 1 }), i010({ 0, 1, 0 }), i011({ 0, 1, 1 }),
                     i100({ 1, 0, 0 }), i101({ 1, 0, 1 }), i110({ 1, 1, 0 }), i111({ 1, 1, 1 });
        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int3_t i1(i000),   // Offsets for second corner of simplex in (i,j,k) coords
               i2(i000);   // Offsets for third corner of simplex in (i,j,k) coords
        if (v0[0] >= v0[1]) {
            if      (v0[1] >= v0[2]) { i1 = i100; i2 = i110; }
            else if (v0[0] >= v0[2]) { i1 = i100; i2 = i101; }
            else                     { i1 = i001; i2 = i101; }
        } else {
            if      (v0[1] <  v0[2]) { i1 = i001; i2 = i011; }
            else if (v0[0] <  v0[2]) { i1 = i010; i2 = i011; }
            else                     { i1 = i010; i2 = i110; }
        }
        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        // c = 1/6.
        
        auto v1(v0 - i1 + 1. * G3);  // Offsets for second corner
        auto v2(v0 - i2 + 2. * G3);  // Offsets for third corner
        auto v3(v0 - 1. + 3. * G3);  // Offsets for fourth corner

        // Work out the hashed gradient indices of the four simplex corners
        i0 &= 255;

        int i = i0[0], j = i0[1], k = i0[2],
            a = i1[0], b = i1[1], c = i1[2],
            d = i2[0], e = i2[1], f = i2[2];
        
        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        
        return 32. * (_corner(v0, grad_p[i +     perm[j +     perm[k    ]]]) +
                      _corner(v1, grad_p[i + a + perm[j + b + perm[k + c]]]) +
                      _corner(v2, grad_p[i + d + perm[j + e + perm[k + f]]]) +
                      _corner(v3, grad_p[i + 1 + perm[j + 1 + perm[k + 1]]]));
    }
    
private:

    inline static double fade(double t) { return t * t * t * (t * (t * 6 - 15) + 10); }
    inline static double lerp(double a, double b, double t) { return (1 - t) * a + t * b; }
    
    static const vec3_t grad3[12];
    
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
    
    static constexpr const int3_t i000({ 0, 0, 0 }), i001({ 0, 0, 1 }), i010({ 0, 1, 0 }), i011({ 0, 1, 1 }),
                     i100({ 1, 0, 0 }), i101({ 1, 0, 1 }), i110({ 1, 1, 0 }), i111({ 1, 1, 1 });
    
    static uint64_t perm[512];
    static vec3_t   grad_p[512];
};

template<typename FloatT>
struct noise_filler
{
    typedef wave::grid<FloatT> grid_t;
    
    simplex       _noise;
    vec3_t        _frequency;
    double        _amplitude;

    inline noise_filler(double seed, const vec3_t &frequency, double amplitude)
        : _noise(seed), _frequency(frequency), _amplitude(amplitude) {}
    inline noise_filler(double seed, double frequency, double amplitude)
    : _noise(seed), _frequency({ frequency, frequency, frequency }), _amplitude(amplitude) {}
    
    inline FloatT at(const wave::point &p) const
    {
        return at(vec2_t({ double(p.x), double(p.y) }));
    }
    
    inline FloatT at(const vec2_t &p) const
    {
        return _noise.evaluate(p * vec2_t({ _frequency[0], _frequency[1] })) * _amplitude;
    }
    
    inline FloatT at(const vec3_t &p) const
    {
        return _noise.evaluate(p * _frequency) * _amplitude;
    }
    
    inline FloatT operator()(grid_t &grid, const wave::point &p) const
    {
        return at(vec3_t({ FloatT(p.x), FloatT(p.y), 0.23 })) + grid.get(p); // add it to existing noise
    }
};

const vec3_t simplex::grad3[] = {
    vec3_t({ 1., 1., 0. }), vec3_t({ -1.,  1., 0. }), vec3_t({  1., -1.,  0. }), vec3_t({ -1., -1.,  0. }),
    vec3_t({ 1., 0., 1. }), vec3_t({ -1.,  0., 1. }), vec3_t({  1.,  0., -1. }), vec3_t({ -1.,  0., -1. }),
    vec3_t({ 0., 1., 1. }), vec3_t({  0., -1., 1. }), vec3_t({  0.,  1., -1. }), vec3_t({  0., -1., -1. })
};

uint64_t simplex::perm[512];
vec3_t   simplex::grad_p[512];

}
