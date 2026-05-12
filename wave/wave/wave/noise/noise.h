#pragma once

#include "grid.h"
#include "std_array.h"
#include "functors.h"
#include "simplex.h"
#include "core_math.h"

namespace noise {

typedef std::array<double, 2> vec2_t;
typedef std::array<double, 3> vec3_t;
typedef std::array<double, 4> vec4_t;

template<typename FloatT>
struct noise_parameters
{
    typedef std::array<FloatT, 3> vec3_t;

    FloatT   frequency;
    FloatT   amplitude;
    FloatT   billowed;
    FloatT   ridged;
    vec3_t   scale;
    FloatT   rotate_z;
    vec3_t   translate;
    FloatT   damping_steepness;
    FloatT   damping; // sigmoid
    
    inline noise_parameters(FloatT frequency = FloatT(1), FloatT amplitude = FloatT(1)) :
        frequency(1), amplitude(1), billowed(0), ridged(0), scale({ 1, 1, 1 }), rotate_z(0), translate(0, 0, 0), damping_steepness(15), damping(0) {}
};

template<typename FloatT>
struct noise
{
    typedef noise<FloatT>                 self_t;
    typedef unary_functor<FloatT, vec3_t> functor_t;
    typedef std::array<FloatT, 2>         vec2_t;
    typedef std::array<FloatT, 3>         vec3_t;
    typedef std::array<FloatT, 4>         vec4_t;
    typedef noise_parameters<FloatT>      parameters_t;
    typedef simplex<FloatT>               simplex_t;
    
    simplex_t _noise;

    inline noise(FloatT seed) : _noise(seed) {}
       
    inline FloatT at(const vec3_t &p, const parameters_t &np) const
    {
        // input & domain warping
        vec3_t v(p * np.frequency * np.scale);
        if (np.rotate_z != 0) {
            FloatT theta = np.rotate_z * 2 * math::PI;
            FloatT cth = std::cos(theta), sth = std::sin(theta);
            v = { v[0] * cth - v[1] * sth, v[0] * sth + v[1] * cth, v[2] };
        }
        v += np.translate;

        // evaluate noise
        FloatT value  = _noise.evaluate(v); // 2d or 3d noise
        
        if (np.billow >= 0 || np.ridge >= 0) {
            FloatT billow = std::abs(value), ridge = FloatT(1) - billow;
            value = lerp(lerp(value, billow, np.billowed), ridge, np.ridged);
        }

        // damping/logistic function scaled from -1 - 1
        if (np.damping != 0) {
            FloatT d(1 + exp(-np.damping_steepness * value));
            value = lerp(value, 2 / d - 1, np.damping);
        }

        // amplitude
        return value * np.amplitude;
    }
};

template<typename FloatT>
struct noise_filler
{
    typedef noise_filler<FloatT>          self_t;
    typedef wave::grid<FloatT>            grid_t;
    typedef unary_functor<double, vec3_t> functor_t;
    typedef simplex<FloatT>               simplex_t;
    
    simplex_t        _noise;
    vec3_t           _frequency;
    double           _amplitude;
    const functor_t *_amplitude_functor;

    inline noise_filler(double seed, const vec3_t &frequency, double amplitude = 1.0)
        : _noise(seed), _frequency(frequency), _amplitude(amplitude), _amplitude_functor(nullptr) {}
    inline noise_filler(double seed, double frequency, double amplitude = 1.0)
        : _noise(seed), _frequency({ frequency, frequency, frequency }), _amplitude(amplitude), _amplitude_functor(nullptr) {}
    
    inline self_t &set_amplitude_functor(const functor_t *functor)
    {
        _amplitude_functor = functor;
        return *this;
    }
    
    inline FloatT at(const point2i &p) const
    {
        return at(vec2_t({ double(p.x), double(p.y) }));
    }
    
    inline FloatT at(const vec2_t &p) const
    {
        return _noise.evaluate(vec3_t({ p[0], p[1], 0.023 }) * _frequency) * amplitude(p);
        //return _noise.evaluate(p * vec2_t({ _frequency[0], _frequency[1] })) * amplitude(p));
    }
    
    inline FloatT at(const vec3_t &p) const
    {
        return _noise.evaluate(p * _frequency) * amplitude(p);
    }
    
    inline FloatT amplitude(const vec3_t &p) const
    {
        return _amplitude_functor ? (*_amplitude_functor)(p) : _amplitude;
    }
    
    inline FloatT amplitude(const vec2_t &p) const
    {
        return amplitude(vec3_t({ p[0], p[1], p[1] }));
    }
    
    inline FloatT operator()(grid_t &grid, const point2i &p) const
    {
        return at(p) + grid.get(p); // add it to existing noise
        //return at(vec3_t({ FloatT(p.x), FloatT(p.y), 0.23 })) + grid.get(p); // add it to existing noise
    }
};

}
