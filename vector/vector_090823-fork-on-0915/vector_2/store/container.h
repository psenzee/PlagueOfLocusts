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

#include "simple_timer.h"
#include "stl.h"
#include "generators.h"
#include "fixed_vector.h"
#include "file.h"
#include "file_map.h"
#include "simd.h"

template <size_t M, typename FloatT = float, typename TraitsT = fixed_vector::traits<M, FloatT, fixed_vector::metadata__default<FloatT> > >
struct container : public fixed_vector::container<M, FloatT, TraitsT>
{
    typedef FloatT                                    value_type;
    typedef typename fixed_vector::traits<M, FloatT>  traits_t;
    typedef typename traits_t::metadata_t             metadata_t;
    typedef typename metadata_t::id_t                 id_t;
    typedef typename traits_t::record_t               record_t;
    typedef typename traits_t::vector_t               vector_t;
    typedef typename traits_t::dynamic_t              dynamic_t;
    typedef std::vector<record_t>                     data_t;
    typedef std::pair<value_type, size_t>             value_index_t;
    typedef std::pair<value_index_t, value_index_t>   range_t;
    typedef vector_generator<float_t>                 generator_t;
    typedef container<M, value_type>                  self_t;

    static constexpr size_t COUNT = M;

    data_t _data;
    
    inline self_t         &add(const vector_t &v)       { return add(record_t(id_t(_data.size()), v)); }
    inline self_t         &add(const record_t &r)       { _data.push_back(r); return *this; }
    inline self_t         &add(const dynamic_t &v)      { vector_t vo; return add(arrays::fixed(v, vo)); }
    inline size_t          size()                 const { return _data.size(); }
    inline self_t         &finish_add()                 { return *this; }
    inline const vector_t &at(size_t i)           const { return _data[i].data; }
    inline const record_t *data()                 const { return &_data; }

    inline std::ostream   &print(std::ostream &os, size_t index, int count = -1)                           const { return arrays::print(os, _data[index].data, count); }
     
    inline value_type      distance_sq(const vector_t &find, size_t index)                                 const { return simd::dist_sq(find, _data[index].data); }
    inline value_type      distance_sq(const dynamic_t &find, size_t index)                                const { return simd::dist_sq(find, _data[index].data); }
    inline value_type      distance_sq(size_t index_a, size_t index_b)                                     const { return distance_sq(_data[index_a].data, index_b); }
    
    inline value_type      distance_sq_limit(const vector_t &find, size_t index, value_type limit_sq)      const { return simd::dist_sq_limit(find, _data[index].data, limit_sq); }
    inline value_type      distance_sq_limit(const dynamic_t &find, size_t index, value_type limit_sq)     const { return simd::dist_sq_limit(find, _data[index].data, limit_sq); }
    inline value_type      distance_sq_limit(size_t index_a, size_t index_b, value_type limit_sq)          const { return distance_sq_limit(_data[index_a].data, index_b, limit_sq); }

    inline value_type      distance(const vector_t &find, size_t index)                                    const { return sqrt(distance_sq(find, index)); }
    inline value_type      distance(const dynamic_t &find, size_t index)                                   const { return sqrt(distance_sq(find, index)); }
    inline value_type      distance(size_t index_a, size_t index_b)                                        const { return sqrt(distance_sq(index_a, index_b)); }

    value_index_t linear_search(const vector_t &find) const
    {
        value_index_t minimum_sq(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _data.size(); i != sz; ++i) {
            value_index_t current(distance_sq_limit(find, i, minimum_sq.first), i);
            if (current.first < minimum_sq.first) minimum_sq = current;
        }
        return std::pair<value_type, size_t>(sqrt(minimum_sq.first), minimum_sq.second);
    }

    inline value_index_t linear_search(const dynamic_t &find) const { return linear_search(traits_t::fixed(find)); }

    void generate(generator_t &generator, size_t count)
    {
        std::cout << "[container generate + store vectors] : [" << count << " vectors of dimension " << COUNT << "] ... ";
        simple_timer t;
        generator.set_size(COUNT);
        for (size_t i = 0; i < count; i++) add(generator.vector());
        t.print_elapsed("done [container generate + store vectors]");
    }
    
    struct _compare { inline bool operator()(const record_t &a, const record_t &b) const { return a.meta.norm < b.meta.norm; } };
    
    void sort_by_norm()
    {
        std::cout << "[sort by norm] ... ";
        simple_timer t;
        _compare c;
        std::sort(_data.begin(), _data.end(), c);
        t.print_elapsed("done [sort by norm]");
    }
    
    ssize_t save(const char *filename) const
    {
        std::cout << "[saving to " << filename << "] ... ";
        simple_timer t;
        ssize_t result = file::write_file(filename, _data.data(), _data.size() * sizeof(_data[0]));
        t.print_elapsed("done [saving]");
        return result;
    }
};

template <size_t M, typename FloatT = float, typename TraitsT = fixed_vector::traits<M, FloatT, fixed_vector::metadata__default<FloatT> > >
struct file_map_container : public fixed_vector::container<M, FloatT, TraitsT>
{
    typedef FloatT                                    value_type;
    typedef typename fixed_vector::traits<M, FloatT>  traits_t;
    typedef typename traits_t::metadata_t             metadata_t;
    typedef typename metadata_t::id_t                 id_t;
    typedef typename traits_t::record_t               record_t;
    typedef typename traits_t::vector_t               vector_t;
    typedef typename traits_t::dynamic_t              dynamic_t;
    typedef const record_t *                          data_t;
    typedef std::pair<value_type, size_t>             value_index_t;
    typedef std::pair<value_index_t, value_index_t>   range_t;
    typedef file_map_container<M, value_type>         self_t;
    
    static constexpr size_t COUNT = M;
    
    data_t    _data;
    size_t    _count;
    file_map *_file_map;
    
    inline file_map_container() : _data(nullptr), _count(0), _file_map(0) {}
    inline ~file_map_container() { close(); }
    
    inline self_t         &add(const vector_t &v)       { /* do nothing, read only */ return *this; }
    inline self_t         &add(const record_t &r)       { /* do nothing, read only */ return *this; }
    inline self_t         &add(const dynamic_t &v)      { /* do nothing, read only */ return *this;  }
    inline size_t          size()                 const { return _count; }
    inline self_t         &finish_add()                 { return *this; }
    inline const vector_t &at(size_t i)           const { return _data[i].data; }
    
    inline std::ostream   &print(std::ostream &os, size_t index, int count = -1)                           const { return arrays::print(os, at(index), count); }
    
    inline value_type      distance_sq(const vector_t &find, size_t index)                                 const { return arrays::distance_sq(find, at(index)); }
    inline value_type      distance_sq(const dynamic_t &find, size_t index)                                const { return arrays::distance_sq(find, at(index)); }
    inline value_type      distance_sq(size_t index_a, size_t index_b)                                     const { return arrays::distance_sq(at(index_a), at(index_b)); }
    
    inline value_type      distance_sq_limit(const vector_t &find, size_t index, value_type limit_sq)      const { return arrays::distance_sq_limit(find, at(index), limit_sq); }
    inline value_type      distance_sq_limit(const dynamic_t &find, size_t index, value_type limit_sq)     const { return arrays::distance_sq_limit(find, at(index), limit_sq); }
    inline value_type      distance_sq_limit(size_t index_a, size_t index_b, value_type limit_sq)          const { return arrays::distance_sq_limit(at(index_a), at(index_b), limit_sq); }
    
    inline value_type      distance(const vector_t &find, size_t index)                                    const { return sqrt(distance_sq(find, index)); }
    inline value_type      distance(const dynamic_t &find, size_t index)                                   const { return sqrt(distance_sq(find, index)); }
    inline value_type      distance(size_t index_a, size_t index_b)                                        const { return sqrt(distance_sq(index_a, index_b)); }

    value_index_t linear_search(const vector_t &find) const
    {
        //arrays::print(std::cout << "find: ", find);
        value_index_t minimum_sq(std::numeric_limits<value_type>::max(), 0);
        for (size_t i = 0, sz = _count; i != sz; ++i) {
            value_index_t current(distance_sq_limit(find, i, minimum_sq.first), i);
            if (current.first < minimum_sq.first) minimum_sq = current;
        }
        //arrays::print(std::cout, at(minimum_sq.second));
        //std::cout << " dist " << minimum_sq.first << " id " << minimum_sq.second << std::endl;
        return std::pair<value_type, size_t>(sqrt(minimum_sq.first), minimum_sq.second);
    }
    
    inline value_index_t linear_search(const dynamic_t &find) const { return linear_search(traits_t::fixed(find)); }
    
    void close()
    {
        if (_file_map) {
            _file_map->close();
            delete _file_map;
        }
        _data = nullptr;
        _file_map = 0;
        _count = 0;
    }
    
    bool load(const char *filename)
    {
        close();
        _file_map = new file_map(filename);
        if (!_file_map->open()) {
            std::cout << "error: mapping file failed " << _file_map->message() << std::endl;
            close();
            return false;
        }
        _data = reinterpret_cast<const record_t *>(_file_map->data());
        if (!_data) {
            std::cout << "error: null pointer returned for mapped file" << std::endl;
            close();
            return false;
        }
        _count = _file_map->file_size() / sizeof(record_t);
        return true;
    }
};
