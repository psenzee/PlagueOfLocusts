#pragma once

#include <iostream>

#include "noise.h"
#include "grid.h"

namespace noise
{

typedef std::array<double, 3> vec3_t;
typedef std::array<double, 2> vec2_t;

struct amp_functor : public unary_functor<double, vec3_t>
{
    double _period, _inv_period, _amplitude;

    inline amp_functor(double period, double base_amplitude) : _period(period), _inv_period(1. / _period), _amplitude(base_amplitude)  {}
    
    double operator()(const vec3_t &v) const
    {
        double t = std_array::magnitude(v) - _period;
        return std::abs(std::sin(t * 3.14159265 * 0.5 * _inv_period) * _amplitude + _amplitude);
    }
};

template <typename T>
struct octave
{
    double frequency, amplitude;
};

template <typename T>
void octave_noise(
                  grid::grid<T> &grid,
                  const vec2_t &offset,
                  size_t octaves,
                  T frequency,
                  std::mt19937 mt,
                  T amplitude = 1.0,
                  T frequency_multiplier_per_octave = 2.0,
                  T amplitude_multiplier_per_octave = 0.5
                  ) {
    for (size_t i = 0; i < octaves; i++) {
        noise_filler<T> nf(mt(), frequency, amplitude);
        amp_functor af(grid.size().w, amplitude * 1.75);
        //nf.set_amplitude_functor(&af);
        fill(grid, nf);
        frequency *= frequency_multiplier_per_octave;
        amplitude *= amplitude_multiplier_per_octave;
    }
}

template <typename T, size_t N>
void octave_noise(
                  grid::grid<T> &grid,
                  const vec2_t &offset,
                  const std::array<T, N> &frequencies,
                  const std::array<T, N> &amplitudes,
                  std::mt19937 mt
                  ) {
    for (size_t i = 0; i < N; i++) {
        noise_filler<T> nf(mt(), frequencies[i], amplitudes[i]);
        fill(grid, nf);
    }
}

template <typename T>
void octave_noise(grid::grid<T> &grid, const vec2_t &offset, const std::vector< octave<T> > &octaves, std::mt19937 mt)
{
    for (const auto &octave : octaves) {
        noise_filler<T> nf(mt(), octave.frequency, octave.amplitude);
        fill(grid, nf);
    }
}

}
