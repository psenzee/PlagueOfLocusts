#pragma once

#include <cstddef>
#include <iostream>
#include <iomanip>

// templated powers for positive integer exponents
template <typename T, unsigned N> constexpr T pow_ui(T v) { T t = pow_ui<T, N / 2>(v); return t * t * pow_ui<T, N % 2>(v); }
#define _(T) \
template <> constexpr T pow_ui<T, 0>(T v) { return T(1); } \
template <> constexpr T pow_ui<T, 1>(T v) { return v; }
_(double) _(float) _(size_t) _(int64_t) _(uint64_t) _(int32_t) _(uint32_t) _(int16_t)  _(uint16_t) _(int8_t) _(uint8_t)
#undef _

template <typename T, unsigned N> void test_pow_ui()
{
    T values[] = { T(23.2), T(0), T(14.01), T(1024) };
    for (size_t i = 0; i < 4; i++) {
        T v = values[i], a = pow_ui<T, N>(v), b = pow(v, N);
        if (a != b)
            std::cout << "error [" << v << "^" << N << "] " << std::fixed << std::setw(20) << a << " != " << b << std::endl;
    }
}
