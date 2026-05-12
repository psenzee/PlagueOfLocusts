#pragma once

#include "std_array.h"

namespace gradient
{

template <typename T, std::size_t N> constexpr std::array<T, N> gradient(
    T t, const std::array<T, N> &exposure, const std::array<T, N> &contrast,
    const std::array<T, N> &frequency, const std::array<T, N> &offsets
) {
    return contrast * std_array::cos(std_array::tau<T, N>() * (frequency * t + offsets)) + exposure;
}

template <typename T, std::size_t N> struct palette: public palette<T, N> {}

template <typename T, std::size_t N> constexpr std::array<T, N> gradient(T t, const palette<T, N> &pal)
{
    return gradient(t, pal[0], pal[1], pal[2], pal[3]);
}

template <typename T, std::size_t N> constexpr palette<T, N> palette_sea_to_forest_0()           { constexpr auto h(std_array::half<T, N>()); return { h, h, { T(1.8), T(1.95), T(0.82) }, std_array::zero<T, N>() }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_sea_to_forest_1()           { constexpr auto h(std_array::half<T, N>()); return { h, h, { T(1.8), T(1.95), T(0.82) }, std::array<T, N>({ T(0.1), T(0.1), T(-0.1) }) }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_sea_to_shore()              { constexpr auto h(std_array::half<T, N>()); return { h, h, { T(1.), T(1.), T(0.5) }, std::array<T, N>({ T(0.1), T(0.1), T(-0.1) }) }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_desert_to_sea()             { constexpr auto h(std_array::half<T, N>()); return { h, h, { T(0.8), T(0.8), T(0.5) }, std::array<T, N>({ T(0.8), T(0.9), T(0.3) }) }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_forest_pool()               { constexpr auto h(std_array::half<T, N>()); return { h, h, std::array<T, N>({ T(1.), T(1.), T(0.5) }), { T(0.8), T(0.95), T(0.32) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_white_to_black(T direction) { constexpr auto h(std_array::half<T, N>()); return { h, h, h, std_array::fill<T, N>(direction) }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_rainbow()                   { constexpr auto h(std_array::half<T, N>()); return { h, h, std_array::one<T, N>(), { T(0.), T(0.33), T(0.67) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_1()                      { return palette<T, N> { std::array<T, N> { T(0.5), T(0.5), T(0.5) }, { T(0.5), T(0.5), T(0.5) }, { T(1.0), T(1.0), T(1.0) }, { T(0.0), T(0.33), T(0.67) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_2()                      { return palette<T, N> { std::array<T, N> { T(0.5), T(0.5), T(0.5) }, { T(0.5), T(0.5), T(0.5) }, { T(1.0), T(1.0), T(1.0) }, { T(0.0), T(0.10), T(0.20) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_3()                      { return palette<T, N> { std::array<T, N> { T(0.5), T(0.5), T(0.5) }, { T(0.5), T(0.5), T(0.5) }, { T(1.0), T(1.0), T(1.0) }, { T(0.3), T(0.20), T(0.20) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_4()                      { return palette<T, N> { std::array<T, N> { T(0.5), T(0.5), T(0.5) }, { T(0.5), T(0.5), T(0.5) }, { T(1.0), T(1.0), T(0.5) }, { T(0.8), T(0.90), T(0.30) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_5()                      { return palette<T, N> { std::array<T, N> { T(0.5), T(0.5), T(0.5) }, { T(0.5), T(0.5), T(0.5) }, { T(1.0), T(0.7), T(0.4) }, { T(0.0), T(0.15), T(0.20) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_6()                      { return palette<T, N> { std::array<T, N> { T(0.5), T(0.5), T(0.5) }, { T(0.5), T(0.5), T(0.5) }, { T(2.0), T(1.0), T(0.0) }, { T(0.5), T(0.20), T(0.25) } }; }
template <typename T, std::size_t N> constexpr palette<T, N> palette_iq_7()                      { return palette<T, N> { std::array<T, N> { T(0.8), T(0.5), T(0.4) }, { T(0.2), T(0.4), T(0.2) }, { T(2.0), T(1.0), T(1.0) }, { T(0.0), T(0.25), T(0.25) } }; }


}
