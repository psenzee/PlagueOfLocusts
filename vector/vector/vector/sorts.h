#pragma once

#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include <deque>

#include "simple_timer.h"

template <typename ValueVectorT, typename IndexVectorT>
struct values_index
{
    typedef ValueVectorT                      values_t;
    typedef IndexVectorT                      indexes_t;
    typedef typename ValueVectorT::value_type value_type;
    typedef typename IndexVectorT::value_type index_type;
    
    struct less {
        const values_index &data;
        inline less(const values_index &data) : data(data) {}
        inline auto at(index_type i) const { return data.values[i]; }
        inline bool operator()(index_type a, index_type b) const { return at(a) < at(b); }
    };
    
    const values_t &values;
    indexes_t      &indexes;

    inline values_index(const values_t &values, indexes_t &indexes) : values(values), indexes(indexes) {}

    void                     index()                        { indexes.resize(values.size()); for (index_type i = 0; i < values.size(); i++) indexes[i] = i; }
    inline void              swap(size_t i, size_t j)       { index_type t = indexes[i]; indexes[i] = indexes[j]; indexes[j] = t; }
    inline size_t            size()                   const { return indexes.size(); }
    inline const value_type &at(size_t i)             const { return values[indexes[i]]; }
    inline const value_type &operator[](size_t i)     const { return values[indexes[i]]; }

    void                     sort()                         { simple_timer t; less c(*this); std::sort(indexes.begin(), indexes.end(), c); t.print_elapsed("sort"); }
    inline bool              is_sorted()              const { less c(*this); return std::is_sorted(indexes.begin(), indexes.end(), c); }

    std::ostream &print(std::ostream &os, size_t count = 50) const
    {
        size_t sz = indexes.size();
        for (size_t i = 0; i < count; i++) os << at(i) << " ";
        os << "... ";
        for (size_t i = sz - count; i < sz; i++) os << at(i) << " ";
        return os << std::endl;
    }
};

template <typename IntT>
inline int first_unequal_bit(int hi_bit, IntT a, IntT b)
{
    if (hi_bit < 0) return -1;
    IntT mask = IntT(1) << hi_bit;
    while (mask) {
        if ((mask & a) != (mask & b)) return hi_bit;
        mask >>= 1;
        hi_bit--;
    }
    return hi_bit;
}

template <typename ValueT, typename KeyT>
struct bit_key_adapter
{
    typedef ValueT   value_type;
    typedef KeyT     key_type;
    inline key_type operator()(const value_type &v) const { return v; }
};

template <typename VectorIndexT, typename KeyT>
struct radix_sort
{
    typedef VectorIndexT                          values_index_t;
    typedef KeyT                                  key_t;
    typedef typename VectorIndexT::value_type     value_type;
    typedef typename VectorIndexT::index_type     index_t;
    typedef bit_key_adapter<value_type, key_t>    key_adapter_t;
    typedef std::pair<key_t, key_t>               bit_extremes_t;
    
    inline radix_sort(values_index_t &vi) : _values_index(vi) {}
    
    void sort(size_t start, size_t end) { _sort(sizeof(key_t) * 8 - 1, _values_index, start, end); }
    
    void sort()
    {
        simple_timer t;
        sort(0, _values_index.size());
        t.print_elapsed("radix sort");
    }

private:
    
    values_index_t _values_index;
    
    enum { FALLBACK_COUNT = 128 * 1024 };
    
    void _sort(int bit, values_index_t &vi, size_t start, size_t end)
    {
        size_t count = end - start;
        if (bit < 0 || count <= 1) return;
        if (count < FALLBACK_COUNT) {
            _subsort(vi, start, end);
            return;
        }
        bit_extremes_t info = _bit_extremes(bit, vi, start, end);
        bit = first_unequal_bit(bit, info.first, info.second);
        key_t mask(key_t(1) << bit);
        size_t zeros = 0;
        for (size_t i = start; i != end; ++i)
            zeros += !(mask & _key(_values_index, i));
        if (!zeros || count == zeros) {
            _sort(bit - 1, _values_index, start, end);
            return;
        }
        size_t mid = start + zeros, s = start, t = mid;
        while (zeros) {
            key_t u(_key(_values_index, s)),
                  v(_key(_values_index, t));
            if (!(u & mask)) {
                zeros--; s++;
            } else if (v & mask) {
                t++;
            } else {
                zeros--;
                vi.swap(s, t);
                s++; t++;
            }
        }
        if (bit > 0) {
            bit--;
            _sort(bit, _values_index, start, mid);
            _sort(bit, _values_index, mid, end);
        }
    }

    inline void _subsort(values_index_t &vi, size_t start, size_t end)
    {
        typename values_index_t::less c(vi);
        std::sort(vi.indexes.begin() + start, vi.indexes.begin() + end, c);
    }
    
    key_adapter_t _key_adapter;
    
    inline key_t   _key(values_index_t &vi, size_t index)  const { return _key_adapter(vi[index]); }
    bit_extremes_t _bit_extremes(int bit, values_index_t &vi, size_t start, size_t end)
    {
        key_t o(1), z(0), mn(~z), mx(0), mask(o << bit), m(mask | (mask - 1));
        for (size_t i = start; i != end; ++i) { key_t k(_key(_values_index, i) & m); mn &= k; mx |= k; }
        return bit_extremes_t(mn, mx);
    }
};

template <typename VectorIndexT, typename KeyT>
struct bucket_sort
{
    typedef VectorIndexT                          values_index_t;
    typedef KeyT                                  key_t;
    typedef uint32_t                              uint_t;
    typedef typename VectorIndexT::value_type     value_type;
    typedef typename VectorIndexT::index_type     index_t;
    typedef bit_key_adapter<value_type, key_t>    key_adapter_t;
    typedef std::pair<key_t, key_t>               bit_extremes_t;
    
    enum { BITS = 6, RANGE = 1 << BITS, MASK = RANGE - 1 };
    
    inline bucket_sort(values_index_t &vi) : _values_index(vi) {}
    
    void sort(size_t start, size_t end) { _sort(sizeof(key_t) * 8, _values_index, uint_t(start), uint_t(end)); }
    
    void sort() { simple_timer t; sort(0, _values_index.size()); t.print_elapsed("bucket sort"); }

private:
    
    struct _bins
    {
        typedef std::array<uint32_t, RANGE + 1> bins_t;

        bins_t counts, iters, starts;
        uint_t last;
        
        inline void clear_counts() { last = 0; counts = { 0 }; starts = { 0 }; }
        inline void compute_starts(uint_t start)
        {
            for (uint_t i = 0; i <= last + 1; i++) {
                starts[i] = start;
                start += counts[i];
            }
            iters = starts;
        }
        inline void increment_count(uint_t bin)
        {
            if (bin > last) last = bin;
            counts[bin]++;
        }
    };
    
    values_index_t _values_index;
    
    enum { FALLBACK_COUNT = 0 };
    
    void _sort(int bit, values_index_t &vi, uint_t start, uint_t end)
    {
        if (bit <= 0 || end <= start) return;
        
        if (end - start < FALLBACK_COUNT) {
            _subsort(vi, start, end);
            return;
        }

        uint_t shift = bit - BITS, mask(MASK);
        mask <<= shift;
        
        _bins bins;
        bins.clear_counts();
        for (uint_t i = start; i != end; ++i)
            bins.increment_count((mask & _key(vi, i)) >> shift);
        bins.compute_starts(start);

        uint_t curr = 0;
        for (uint_t i = start; i < end; ) {
            if (i >= bins.starts[curr + 1]) { ++curr; if (curr > bins.last) break; continue; }
            // jump to the next bucket
            uint_t iter = bins.iters[curr];
            if (i < iter) { i = iter; continue; }

            uint_t bid = (_key(vi, i) & mask) >> shift;
            if (bid != curr) vi.swap(i, bins.iters[bid]);
            else              ++i;
            --bins.counts[bid];
            ++bins.iters[bid];
        }
        bit -= BITS;
        if (bit <= 0) return;
        // this can be parallelized
        for (uint_t i = 1; i <= bins.last + 1; i++) {
            uint_t begin = bins.starts[i - 1], end = bins.starts[i];
            if (end > begin + 1) _sort(bit, vi, begin, end);
        }
    }
    
    inline void _subsort(values_index_t &vi, size_t start, size_t end)
    {
        typename values_index_t::less c(vi);
        std::sort(vi.indexes.begin() + start, vi.indexes.begin() + end, c);
    }
    
    key_adapter_t _key_adapter;
    
    inline key_t   _key(values_index_t &vi, size_t i)  const { return _key_adapter(vi[i]); }
    bit_extremes_t _bit_extremes(int bit, values_index_t &vi, size_t start, size_t end)
    {
        key_t o(1), z(0), mn(~z), mx(0), mask(o << bit), m(mask | (mask - 1));
        for (size_t i = start; i != end; ++i) { key_t k(_key(_values_index, i) & m); mn &= k; mx |= k; }
        return bit_extremes_t(mn, mx);
    }
};

template <typename VectorIndexT, typename KeyT>
struct bucket_sort_parallel
{
    typedef VectorIndexT                          values_index_t;
    typedef KeyT                                  key_t;
    typedef uint32_t                              uint_t;
    typedef typename VectorIndexT::value_type     value_type;
    typedef typename VectorIndexT::index_type     index_t;
    typedef bit_key_adapter<value_type, key_t>    key_adapter_t;
    typedef std::pair<key_t, key_t>               bit_extremes_t;
    
    enum { BITS = 6, RANGE = 1 << BITS, MASK = RANGE - 1 };
    
    inline bucket_sort_parallel(values_index_t &vi) : _values_index(vi) {}
    
    void sort(size_t start, size_t end) { _sort(sizeof(key_t) * 8, 0, _values_index, uint_t(start), uint_t(end)); }
    
    void sort() { simple_timer t; sort(0, _values_index.size()); t.print_elapsed("bucket sort"); }

private:
    
    struct _bins
    {
        typedef std::array<uint32_t, RANGE + 1> bins_t;

        bins_t counts, iters, starts;
        uint_t last;
        
        inline void clear_counts() { last = 0; counts = { 0 }; starts = { 0 }; }
        inline void compute_starts(uint_t start)
        {
            for (uint_t i = 0; i <= last + 1; i++) {
                starts[i] = start;
                start += counts[i];
            }
            iters = starts;
        }
        inline void increment_count(uint_t bin)
        {
            if (bin > last) last = bin;
            counts[bin]++;
        }
    };
    
    values_index_t _values_index;
    
    struct _task
    {
        int bit;
        uint_t level, start, end;
        values_index_t &vi;
        
        inline _task(int bit, uint_t level, values_index_t &vi, uint_t start, uint_t end) : bit(bit), level(level), vi(vi), start(start), end(end) {}
    };
    
    std::deque<_task> _tasks;
    
    enum { FALLBACK_COUNT = 0 };
    
    void _sort(int bit, uint_t level, values_index_t &vi, uint_t start, uint_t end)
    {
        if (bit <= 0 || end <= start) return;
        
        if (end - start < FALLBACK_COUNT) {
            _subsort(vi, start, end);
            return;
        }

        uint_t shift = bit - BITS, mask(MASK);
        mask <<= shift;
        
        _bins bins;
        bins.clear_counts();
        for (uint_t i = start; i != end; ++i)
            bins.increment_count((mask & _key(vi, i)) >> shift);
        bins.compute_starts(start);

        uint_t curr = 0;
        for (uint_t i = start; i < end; ) {
            if (i >= bins.starts[curr + 1]) { ++curr; if (curr > bins.last) break; continue; }
            // jump to the next bucket
            uint_t iter = bins.iters[curr];
            if (i < iter) { i = iter; continue; }

            uint_t bid = (_key(vi, i) & mask) >> shift;
            if (bid != curr) vi.swap(i, bins.iters[bid]);
            else              ++i;
            --bins.counts[bid];
            ++bins.iters[bid];
        }
        bit -= BITS;
        if (bit <= 0) return;
        _dispatch_sort(bit, bins, level + 1, vi);
    }
    
    void _dispatch_sort(int bit, const _bins &bins, uint_t level, values_index_t &vi)
    {
        // this can be parallelized
        for (uint_t i = 1; i <= bins.last + 1; i++) {
            uint_t begin = bins.starts[i - 1], end = bins.starts[i];
            if (end > begin + 1) _sort(bit, level + 1, vi, begin, end);
        }
    }
    
    /*
     unsigned int num_threads = std::thread::hardware_concurrency();
     std::cout << "number of threads " << num_threads << std::endl;
     simple_timer td;
     enum { K_CLOSEST = 8, RANKS = 8 };
     std::vector<std::thread> threads;
     std::vector<finder_t *> finders;
     for (size_t i = 0; i < std::min(size_t(num_threads), size_t(RANKS - 2)); i++) {
         finder_t *f = new finder_t(keys, container, K_CLOSEST, i + 1);
         finders.push_back(f);
         threads.push_back(std::thread(
             [i](std::vector<float_t> v, const key_index<key_t> &keys, const container_t &container) -> void {
                 finder_t finder(keys, container, K_CLOSEST, i + 1); finder.start(v); },
               v, std::ref(keys), std::ref(container)));
     }
     for (auto &t : threads) {
         if (t.joinable()) {
             t.join();
         }
     }
     td.print_elapsed("  finder start parallel");
     */
    
    inline void _subsort(values_index_t &vi, size_t start, size_t end)
    {
        typename values_index_t::less c(vi);
        std::sort(vi.indexes.begin() + start, vi.indexes.begin() + end, c);
    }
    
    key_adapter_t _key_adapter;
    
    inline key_t   _key(values_index_t &vi, size_t i)  const { return _key_adapter(vi[i]); }
    bit_extremes_t _bit_extremes(int bit, values_index_t &vi, size_t start, size_t end)
    {
        key_t o(1), z(0), mn(~z), mx(0), mask(o << bit), m(mask | (mask - 1));
        for (size_t i = start; i != end; ++i) { key_t k(_key(_values_index, i) & m); mn &= k; mx |= k; }
        return bit_extremes_t(mn, mx);
    }
};

template <typename VectorT, typename IndexT>
struct compare_index
{
    const VectorT &_data;
    inline compare_index(const VectorT &data) : _data(data) {}
    inline bool operator()(IndexT a, IndexT b) const { return _data[a] < _data[b]; }
};

struct float_index_bucket_sort
{
    typedef float                    float_t;
    typedef uint32_t                 index_t;
    typedef std::vector<float_t>     data_t;
    typedef std::vector<index_t>     indexes_t;
    typedef std::array<index_t, 512> buckets_t;

    const data_t &_values;
    indexes_t     _indexes;
    buckets_t     _iters, _counts;
    
    inline float_index_bucket_sort(const data_t &values) : _values(values) {}
    
    const indexes_t &sort()
    {
        simple_timer t;
        _iters = { 0 };
        _counts = { 0 };
        index_t sz(index_t(_values.size())), csz(index_t(_counts.size()));
        _indexes.resize(sz);
        for (index_t i = 0; i != sz; ++i) _indexes[i] = i;
        for (auto v : _values) _counts[_isnexp(v)]++;
        index_t run = 0;
        for (size_t i = 0; i != csz; ++i) {
            _iters[i] = run;
            run += _counts[i];
        }
        buckets_t iters(_iters);
        for (index_t i = 0; i != sz; ++i) {
            index_t se = _isnexp(_values[i]), k = _iters[se];
            _indexes[k] = i;
            _iters[se]++;
        }
        for (size_t i = 1; i != csz; ++i) {
            index_t a = iters[i - 1], b = iters[i];
            _subsort(_indexes, a, b);
        }
        t.print_elapsed("bucket sort");
        return _indexes;
    }
    
    inline const indexes_t &indexes() const { return _indexes; }

private:
    
    void _subsort(indexes_t &indexes, index_t a, index_t b)
    {
        compare_index<data_t, index_t> c(_values);
        std::sort(indexes.begin() + a, indexes.begin() + b, c);
    }
    
    void _bucket_sort()
    {
        
        
    }
    
    union _float_union { float f; uint32_t u; };

    inline static index_t _isnexp(float_t value) { int exp = 0; frexp(value, &exp); return index_t((exp + 127) | (value < 0 ? 0 : 256)); }
};

struct control_sort
{
    typedef float                    float_t;
    typedef uint32_t                 index_t;
    typedef std::vector<float_t>     data_t;
    typedef std::vector<index_t>     indexes_t;

    const data_t &_values;
    indexes_t     _indexes;
    
    inline control_sort(const data_t &values) : _values(values) {}
    
    const indexes_t &sort()
    {
        simple_timer t;
        index_t sz(index_t(_values.size()));
        _indexes.resize(sz);
        for (index_t i = 0; i != sz; ++i) _indexes[i] = i;
        compare_index<data_t, index_t> c(_values);
        std::sort(_indexes.begin(), _indexes.end(), c);
        t.print_elapsed("control sort");
        return _indexes;
    }
    
    inline const indexes_t &indexes() const { return _indexes; }
};

void test_float_index_bucket_sort()
{
    enum { COUNT = 1024 * 1024 * 128 };
    std::vector<float> values;
    values.resize(COUNT);
    double rmax = 10000.0 / RAND_MAX;
    for (size_t i = 0; i != COUNT; i++)
        values[i] = rand() * rmax;
    float_index_bucket_sort sorter_a(values);
    sorter_a.sort();
    control_sort sorter_b(values);
    sorter_b.sort();
}

void test_radix_sort()
{
    typedef std::vector<uint32_t> value_vector_t;
    typedef std::vector<uint32_t> index_vector_t;
    
    enum { SEED = 234234 };

    enum { COUNT = 1024 * 1024 * 128 };
    srand(SEED);
    value_vector_t values_a;
    index_vector_t indexes_a;
    values_index vi_a(values_a, indexes_a);
    values_a.resize(COUNT);
    std::cout << "COUNT " << COUNT << std::endl;
    for (size_t i = 0; i != COUNT; i++)
        values_a[i] = rand() % 1000000000;
    vi_a.index();

    //bucket_sort< values_index<value_vector_t, index_vector_t>, uint32_t > sorter(vi_a);
    bucket_sort_parallel< values_index<value_vector_t, index_vector_t>, uint32_t > sorter(vi_a);
    sorter.sort();
    std::cout << "sorted" << (vi_a.is_sorted() ? " yes" : " no") << std::endl;
    vi_a.print(std::cout, 5);

    srand(SEED);
    value_vector_t values_b;
    index_vector_t indexes_b;
    values_index vi_b(values_b, indexes_b);
    values_b.resize(COUNT);
    std::cout << "COUNT " << COUNT << std::endl;
    for (size_t i = 0; i != COUNT; i++)
        values_b[i] = rand() % 1000000000;
    vi_b.index();
    
    vi_b.sort();
    std::cout << "sorted" << (vi_b.is_sorted() ? " yes" : " no") << std::endl;
    vi_b.print(std::cout, 5);
    
    std::cout << std::endl << std::endl;
}
