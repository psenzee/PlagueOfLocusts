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
#include "arrays.h"

namespace fixed_vector
{

template <typename FloatT>
struct metadata__default
{
    typedef uint32_t                  id_t;
    typedef FloatT                    norm_t;
    typedef metadata__default<FloatT> self_t;

    id_t   id;
    norm_t norm;
    
    inline metadata__default() : id(0), norm(0) {}
    inline self_t &set(id_t id_, norm_t n) { id = id_; norm = n; return *this; }
};

template <size_t M, typename FloatT, typename MetadataT = metadata__default<FloatT> >
struct record
{
    typedef MetadataT                    metadata_t;
    typedef typename metadata_t::id_t    id_t;
    typedef std::array<FloatT, M>        vector_t;
    
    metadata_t meta;
    vector_t   data;
    
    inline record(id_t id, const vector_t &v) : data(v) { meta.set(id, sqrt(arrays::dot(v))); }
};

template <size_t M, typename FloatT, typename MetadataT = metadata__default<FloatT> >
struct traits
{
    typedef FloatT                                  value_type;
    typedef MetadataT                               metadata_t;
    typedef std::array<FloatT, M>                   vector_t;
    typedef record<M, FloatT, MetadataT>            record_t;
    typedef std::vector<FloatT>                     dynamic_t;
    typedef std::pair<value_type, size_t>           value_index_t;
    typedef std::pair<value_index_t, value_index_t> range_t;
    typedef vector_generator<float_t>               generator_t;
    
    static constexpr size_t COUNT = M;
};

template <size_t M, typename FloatT, typename TraitsT = traits<M, FloatT, metadata__default<FloatT> > >
struct container
{
    typedef TraitsT                       traits_t;
    typedef typename traits_t::metadata_t metadata_t;
    typedef typename traits_t::vector_t   vector_t;
    typedef typename traits_t::record_t   record_t;
    typedef typename traits_t::dynamic_t  dynamic_t;
    typedef container<M, FloatT>          self_t;
    
    static constexpr size_t COUNT = M;
    
    virtual ~container();
    
    virtual self_t         &add(const vector_t &v)                                      = 0;
    virtual self_t         &add(const record_t &v)                                      = 0;
    virtual self_t         &add(const dynamic_t &v)                                     = 0;
    virtual size_t          size()                                                const = 0;
    virtual const vector_t &at(size_t i)                                          const = 0;
    virtual self_t         &finish_add()                                                = 0;
    virtual std::ostream   &print(std::ostream &os, size_t index, int count = -1) const = 0;
    
    inline static std::ostream &print(std::ostream &os, const vector_t &v,  int count = -1) { return traits_t::print(os, v, count); }
    inline static std::ostream &print(std::ostream &os, const dynamic_t &v, int count = -1) { return traits_t::print(os, v, count); }
    
};

template <size_t M, typename FloatT, typename TraitsT> container<M, FloatT, TraitsT>::~container<M, FloatT, TraitsT>() {}

template <size_t M, typename FloatT, typename TraitsT = traits<M, FloatT, metadata__default<FloatT> >  >
struct container_index
{
    typedef TraitsT                          traits_t;
    typedef typename traits_t::vector_t      vector_t;
    typedef typename traits_t::value_index_t value_index_t;
    
    virtual ~container_index();

    virtual value_index_t find(const vector_t &find) const = 0;
};

template <size_t M, typename FloatT, typename TraitsT> container_index<M, FloatT, TraitsT>::~container_index<M, FloatT, TraitsT>() {}

template <size_t M, typename FloatT, typename TraitsT = traits<M, FloatT, metadata__default<FloatT> >   >
struct container_linear_search : public container_index<M, FloatT, TraitsT>
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
            value_index_t current(arrays::distance_sq_limit(find, _container.at(i), minimum.first), i);
            if (current.first < minimum.first) minimum = current;
        }
        return minimum;
    }
    
private:

    container_t &_container;
};

template <size_t M, typename FloatT, typename TraitsT = traits<M, FloatT, metadata__default<FloatT> > >
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

template <size_t M, typename FloatT, typename TraitsT = traits<M, FloatT, metadata__default<FloatT> > >
struct basic_container : public container<M, FloatT, TraitsT>
{
    typedef TraitsT                     traits_t;
    typedef typename traits_t::vector_t vector_t;
    typedef typename traits_t::record_t record_t;
    typedef basic_container<M, FloatT>  self_t;
    typedef std::vector<record_t>       data_t;

    data_t _data;

    inline self_t         &add(const vector_t &v)       { return add(record_t(_data.size(), v)); }
    inline self_t         &add(const record_t &r)       { _data.push_back(r); return *this; }
    inline size_t          size()                 const { return _data.size(); }
    inline self_t         &finish_add()                 { return *this; }
    inline const vector_t &at(size_t i)           const { return _data[i]; }
};

}
