#pragma once

#include <iostream>
#include <limits>
#include <cmath>
#include <array>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <cstddef>
#include <queue>
#include <thread>
#include <deque>

#include "stl.h"
#include "simple_timer.h"
#include "generators.h"

namespace fixed_vector
{

template <int M, typename FloatT>
struct traits
{
    typedef FloatT                                  value_type;
    typedef std::array<FloatT, M>                   vector_t;
    typedef std::vector<FloatT>                     dynamic_t;
    typedef std::pair<value_type, size_t>           value_index_t;
    typedef std::pair<value_index_t, value_index_t> range_t;
    typedef vector_generator<float_t>               generator_t;
    
    static constexpr int COUNT = M;
    
    inline static vector_t  fixed(const dynamic_t &v)  { vector_t vo; std::copy(v.begin(), v.end(), vo.begin()); return vo; }
    inline static dynamic_t dynamic(const vector_t &v) { dynamic_t vo; vo.resize(v.size()); std::copy(v.begin(), v.end(),vo.begin()); return vo; }
    static std::ostream    &print(std::ostream &os, const vector_t &v, int count = -1)
    {
        os << "[" << std::setprecision(2);
        count = (count < 0 || count > v.size()) ? int(v.size()) : count;
        for (auto b = v.begin(), i = b, e = i + count; i != e; ++i) os << (i == b ? "" : " ") << *i;
        return os << (v.size() > count ? " ..." : "") << "]";
    }
};

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
inline FloatT normalize(typename TraitsT::vector_t &v)
{
    FloatT norm = dot(v, v);
    if (norm != FloatT(0)) {
        norm = 1 / sqrt(norm);
        for (auto &u : v) u *= norm;
    }
    return norm;
}

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
struct norm_vector
{
    typedef TraitsT                     traits_t;
    typedef FloatT                      value_type;
    typedef typename traits_t::vector_t vector_t;
    
    value_type norm;
    vector_t   vector;
    
    inline norm_vector(value_type norm, const vector_t &v) : norm(norm), vector(v) {}
    inline norm_vector(const vector_t &v)                  : vector(v) { norm = normalize(vector); }
};

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
struct container
{
    typedef TraitsT                      traits_t;
    typedef typename traits_t::vector_t  vector_t;
    typedef typename traits_t::dynamic_t dynamic_t;
    typedef container<M, FloatT>         self_t;
    
    virtual ~container();
    
    virtual self_t         &add(const vector_t &v)                                      = 0;
    virtual self_t         &add(const dynamic_t &v)                                     = 0;
    virtual size_t          size()                                                const = 0;
    virtual const vector_t &at(size_t i)                                          const = 0;
    virtual self_t         &finish_add()                                                = 0;
    virtual std::ostream   &print(std::ostream &os, size_t index, int count = -1) const = 0;
    
    inline static std::ostream &print(std::ostream &os, const vector_t &v,  int count = -1) { return traits_t::print(os, v, count); }
    inline static std::ostream &print(std::ostream &os, const dynamic_t &v, int count = -1) { return traits_t::print(os, v, count); }
    
};

template <int M, typename FloatT, typename TraitsT> container<M, FloatT, TraitsT>::~container<M, FloatT, TraitsT>() {}

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
struct container_index
{
    typedef TraitsT                          traits_t;
    typedef typename traits_t::vector_t      vector_t;
    typedef typename traits_t::value_index_t value_index_t;
    
    virtual ~container_index();

    virtual value_index_t find(const vector_t &find) const = 0;
};

template <int M, typename FloatT, typename TraitsT> container_index<M, FloatT, TraitsT>::~container_index<M, FloatT, TraitsT>() {}

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
struct container_linear_search : public container_index<M, FloatT>
{
    typedef TraitsT                          traits_t;
    typedef FloatT                           value_type;
    typedef container<M, FloatT, TraitsT>    container_t;
    typedef typename traits_t::vector_t      vector_t;
    typedef typename traits_t::value_index_t value_index_t;
    
    inline container_linear_search(container_t &container) : _container(container) {}

    value_index_t find(const vector_t &find) const
    {
        value_index_t minimum(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _container.size(); i != sz; ++i) {
            value_index_t current(::distance_sq_limit(find, _container.at(i), minimum.first), i);
            if (current.first < minimum.first) minimum = current;
        }
        return minimum;
    }
    
private:

    container_t &_container;
};

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
struct container_filler
{
    typedef TraitsT                        traits_t;
    typedef FloatT                         value_type;
    typedef container<M, FloatT>           container_t;
    typedef typename traits_t::generator_t generator_t;
    typedef typename container_t::vector_t vector_t;
    
    inline container_filler(container_t &container, generator_t &generator) : _container(container), _generator(generator) {}

    void fill(size_t count)
    {
        vector_t u;
        size_t dims = u.size(); // careful this only works with std::array, std::vector here returns 0
        std::cout << "container [" << count << " vectors of dimension " << dims << "]" << std::endl;
        simple_timer t;
        _generator.set_size(dims);
        for (size_t i = 0; i < count; i++) {
            const std::vector<value_type> &v = _generator.vector();
            std::copy(v.begin(), v.end(), u.begin());
            _container.add(u);
        }
        _container.finish_add();
        t.print_elapsed("container generate + store vectors");
    }
    
private:

    container_t &_container;
    generator_t &_generator;
};

template <int M, typename FloatT, typename TraitsT = traits<M, FloatT> >
struct basic_container : public container<M, FloatT, TraitsT>
{
    typedef TraitsT                     traits_t;
    typedef typename traits_t::vector_t vector_t;
    typedef basic_container<M, FloatT>  self_t;
    typedef std::vector<vector_t>       data_t;

    data_t _data;
    
    inline self_t         &add(const vector_t &v)       { _data.push_back(v); return *this; }
    inline size_t          size()                 const { return _data.size(); }
    inline self_t         &finish_add()                 { return *this; }
    inline const vector_t &at(size_t i)           const { return _data[i]; }
};

}
