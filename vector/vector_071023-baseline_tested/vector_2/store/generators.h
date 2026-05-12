#pragma once

#include <limits>
#include <cmath>
#include <vector>
#include <cstddef>

#include "stl.h"

template <typename FloatT>
struct real_generator
{
    typedef FloatT               float_t;
    typedef std::vector<float_t> vector_t;
    
    inline float_t real_spread() const
    {
        float_t factor = pow(2, (rand() % 16) - (rand() % 16));
        return real_n1p1() * factor;
    }
    
    inline float_t real_0_1() const { return float_t(rand()) / float_t(RAND_MAX); }
    
    inline float_t real_n1p1() const { int64_t rm = RAND_MAX >> 1, r = rand(); return float_t(r - rm) / float_t(rm); }

    const vector_t &vector(size_t count, vector_t &out) const
    {
        if (out.size() < count) out.resize(count);
        for (size_t i = 0; i < count; i++) out[i] = real_spread();
        return out;
    }
};

template <typename FloatT>
struct vector_generator
{
    typedef FloatT               float_t;
    typedef std::vector<float_t> vector_t;
    
    static constexpr float_t MAX_RANDOM = float_t(5000);

    enum NormalizeMode { NORMALIZE_MODE_NONE = 0, NORMALIZE_MODE_NORMAL = 1, NORMALIZE_MODE_RANDOM_LENGTH = 2 };
    
    real_generator<float_t> _reals;
    size_t                  _size;
    mutable vector_t        _temporary;
    NormalizeMode           _mode;
    
    inline vector_generator(size_t size, NormalizeMode mode = NORMALIZE_MODE_NONE) : _size(size), _mode(mode) {}
    
    inline void set_size(size_t size) { _size = size; }
    inline void set_mode(NormalizeMode mode) { _mode = mode; }
    
    inline const vector_t &vector() const { return vector(_temporary); }
    
    const vector_t &vector(vector_t &v) const
    {
        _reals.vector(_size, v);
        switch (_mode) {
            case NORMALIZE_MODE_NORMAL:        _resize(v); break;
            case NORMALIZE_MODE_RANDOM_LENGTH: _resize(v, _reals.real_0_1() * MAX_RANDOM); break;
            default: break;
        }
        return v;
    }

private:
    
    void _resize(vector_t &v, float_t length = float_t(1.0)) const
    {
        float_t inv_length = float_t(1.0) / sqrt(magnitude_sq(v));
        for (size_t i = 0, sz = v.size(); i != sz; ++i)
            v[i] *= inv_length * length;
    }
};

typedef vector_generator<float>  float_vector_generator;
typedef real_generator<float>    float_real_generator;
typedef vector_generator<double> double_vector_generator;
typedef real_generator<double>   double_real_generator;
