#pragma once

#include <cstddef>
#include "interpolate.h"

enum ColorOrder { RGBA, ABGR, BGRA, ARGB };

template <ColorOrder Order> struct color_order { enum order {}; };

template <> struct color_order<RGBA> { enum order { R = 0, G = 1, B = 2, A = 3 }; };
template <> struct color_order<ABGR> { enum order { A = 0, B = 1, G = 2, R = 3 }; };
template <> struct color_order<BGRA> { enum order { B = 0, G = 1, R = 2, A = 3 }; };
template <> struct color_order<ARGB> { enum order { A = 0, R = 1, G = 2, B = 3 }; };

template <typename T, typename Order> struct color { typedef Order order_t; typedef T value_type; T r, g, b, a; order_t order; };
template <typename T> struct color<T, color_order<RGBA> > { typedef color_order<RGBA> order_t; typedef T value_type; T r, g, b, a; order_t order; };
template <typename T> struct color<T, color_order<ABGR> > { typedef color_order<ABGR> order_t; typedef T value_type; T a, b, g, r; order_t order; };
template <typename T> struct color<T, color_order<BGRA> > { typedef color_order<BGRA> order_t; typedef T value_type; T b, g, r, a; order_t order; };
template <typename T> struct color<T, color_order<ARGB> > { typedef color_order<ARGB> order_t; typedef T value_type; T a, r, g, b; order_t order; };

namespace {
template <typename U> constexpr U        conv(uint8_t v)             { return U(v); }
template <typename U> constexpr U        conv(uint16_t v)            { return U(v); }
template <typename U> constexpr U        conv(float v)               { return U(v); }
template <typename U> constexpr U        conv(double v)              { return U(v); }

template <>           constexpr uint8_t  conv<uint8_t>(float v)      { return uint8_t(v * 255.f); }
template <>           constexpr uint16_t conv<uint16_t>(float v)     { return uint16_t(v * 65535.f); }

template <>           constexpr uint8_t  conv<uint8_t>(double v)     { return uint8_t(v * 255.); }
template <>           constexpr uint16_t conv<uint16_t>(double v)    { return uint16_t(v * 65535.); }

template <>           constexpr uint8_t  conv<uint8_t>(uint16_t v)   { return uint8_t(v >> 8); }
template <>           constexpr float    conv<float>(uint16_t v)     { constexpr float  inv = 1.f / 65535.f; return float(v)  * inv; }
template <>           constexpr double   conv<double>(uint16_t v)    { constexpr double inv = 1.  / 65535.;  return double(v) * inv; }

template <>           constexpr uint16_t conv<uint16_t>(uint8_t v)   { return uint16_t(v) << 8; }
template <>           constexpr float    conv<float>(uint8_t v)      { constexpr float  inv = 1.f / 255.f; return float(v)  * inv; }
template <>           constexpr double   conv<double>(uint8_t v)     { constexpr double inv = 1.  / 255.;  return double(v) * inv; }

                      constexpr uint8_t  clamp(uint8_t v)            { return v; } // can't really clamp uints because they just roll over
                      constexpr uint16_t clamp(uint16_t v)           { return v; } // can't really clamp uints because they just roll over
                      constexpr float    clamp(float v)              { return v > 1.f ? 1.f : (v < 0.f ? 0.f : v); }
                      constexpr double   clamp(double v)             { return v > 1.  ? 1.  : (v < 0.  ? 0.  : v); }
}

template <typename T, ColorOrder Order> constexpr color<T, color_order<Order> > operator*(const color<T, color_order<Order> > &a, double b) { color<T, color_order<Order> > c = a; c.a *= b; c.r *= b; c.g *= b; c.b *= b; return c; }
template <typename T, ColorOrder Order> constexpr color<T, color_order<Order> > operator+(const color<T, color_order<Order> > &a, double b) { color<T, color_order<Order> > c = a; c.a += b; c.r += b; c.g += b; c.b += b; return c; }

template <typename T, ColorOrder Order> constexpr color<T, color_order<Order> > operator*(const color<T, color_order<Order> > &a, const color<T, color_order<Order> > &b) { color<T, color_order<Order> > c = a; c.a *= b.a; c.r *= b.r; c.g *= b.g; c.b *= b.b; return c; }
template <typename T, ColorOrder Order> constexpr color<T, color_order<Order> > operator+(const color<T, color_order<Order> > &a, const color<T, color_order<Order> > &b) { color<T, color_order<Order> > c = a; c.a += b.a; c.r += b.r; c.g += b.g; c.b += b.b; return c; }

template <typename T, typename Order>
constexpr color<T, Order> clamp(const color<T, Order> &ci)
{
    color<T, Order> co;
    co.a = clamp(ci.a);
    co.r = clamp(ci.r);
    co.g = clamp(ci.g);
    co.b = clamp(ci.b);
    return co;
}

template <typename ColorIn, typename ColorOut>
constexpr ColorOut color_to(const ColorIn &ci)
{
    typedef typename ColorOut::value_type out_t;
    ColorOut co;
    co.a = conv<out_t>(ci.a);
    co.r = conv<out_t>(ci.r);
    co.g = conv<out_t>(ci.g);
    co.b = conv<out_t>(ci.b);
    return co;
};

template <typename C>
constexpr uint32_t color_to_u32(const C &u)
{
    typedef typename C::order_t O;
    typedef uint8_t u8;
    return (uint32_t(conv<u8>(u.a)) << (O::A * 8)) |
           (uint32_t(conv<u8>(u.r)) << (O::R * 8)) |
           (uint32_t(conv<u8>(u.g)) << (O::G * 8)) |
           (uint32_t(conv<u8>(u.b)) << (O::B * 8));
};

template <typename T, typename Order>
constexpr color<T, Order> lerp(const color<T, Order> &a, const color<T, Order> &b, double t)
{
    color<T, Order> c;
    c.a = lerp(a.a, b.a, t);
    c.r = lerp(a.r, b.r, t);
    c.g = lerp(a.g, b.g, t);
    c.b = lerp(a.b, b.b, t);
    return c;
}

typedef color<uint8_t,  color_order<RGBA> > color_rgba_u8;
typedef color<uint8_t,  color_order<ARGB> > color_argb_u8;
typedef color<uint8_t,  color_order<ABGR> > color_abgr_u8;
typedef color<uint8_t,  color_order<BGRA> > color_bgra_u8;

typedef color<uint16_t, color_order<RGBA> > color_rgba_u16;
typedef color<float,    color_order<RGBA> > color_rgba_f;
typedef color<double,   color_order<RGBA> > color_rgba_d;

