#pragma once

#include <iostream>
#include <iomanip>
#include <cstddef>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include "stl.h"

template <typename KeyT, typename IndexT>
struct key_bin
{
    typedef KeyT                                        key_t;
    typedef IndexT                                      index_t;
    typedef std::pair<key_t, index_t>                   pair_t;
    typedef std::vector<pair_t>                         vector_t;
    typedef pair_compare_first_less_second_less<pair_t> compare_t;
    typedef pair_compare_first_less<pair_t>             bounds_compare_t;
    typedef typename vector_t::const_iterator           const_iterator;

private:

    vector_t _data;
    bool     _valid;

public:

    inline key_bin() : _valid(false) {}
    
    inline void   validate()                                    { if (!_valid) { compare_t c; std::sort(_data.begin(), _data.end(), c); _valid = true; } }
    inline bool   is_valid()                              const { return _valid; }
    inline auto   begin()                                 const { return _data.begin(); }
    inline auto   end()                                   const { return _data.end(); }
    inline size_t size()                                  const { return _data.size(); }
    inline void   insert(const pair_t &data)                    { _data.push_back(data); _valid = false; }
    inline void   insert(const key_t &key, const index_t &id)   { insert(pair_t(key, id)); }
    
    inline auto   equal_range(const key_t &key)           const { bounds_compare_t c; return std::equal_range(begin(), end(), pair_t(key, 0), c); }
    inline auto   upper_bound(const key_t &key)           const { bounds_compare_t c; return std::upper_bound(begin(), end(), pair_t(key, 0), c); }
    inline auto   lower_bound(const key_t &key)           const { bounds_compare_t c; return std::lower_bound(begin(), end(), pair_t(key, 0), c); }
    
    void test_print()  const
    {
        size_t count = 0;
        for (const auto &p : _data) {
            std::cout << "[" << p.first << " " << p.second << "]" << std::endl;
            count++;
            if (count > 100) return;
        }
    }
};

template <typename KeyT, typename IndexT>
struct key_id_index
{
    typedef KeyT                                        key_t;
    typedef IndexT                                      index_t;
    typedef std::pair<key_t, index_t>                   pair_t;
    typedef key_bin<key_t, index_t>                     bin_t;
    typedef std::unordered_map<key_t, bin_t *>          map_t;
    typedef pair_compare_first_less_second_less<pair_t> compare_t;
    typedef typename bin_t::const_iterator              const_iterator;

private:

    static constexpr size_t KEY_BITS = sizeof(KeyT) * 8, ONE = size_t(1);

public:

    inline key_id_index(size_t category_bits = 12) : _valid(false), _size(0) { _set_bits(category_bits); }
    inline ~key_id_index() { _clean(); }

    inline void         insert(const key_t &key, const index_t &id) { _bin_retrieve_or_create(key)->insert(key, id); }

    inline bin_t       *bin_ptr(const key_t &key)                   { auto i = _data.find(_key(key)); return (i == _data.end()) ? 0 : (*i).second; }
    inline const bin_t &bin(const key_t &key)                 const { auto i = _data.find(_key(key)); return (i == _data.end()) ? _empty : *((*i).second); }

    inline auto         begin(const key_t &key)               const { return bin(key).begin(); }
    inline auto         end(const key_t &key)                 const { return bin(key).end(); }

    inline auto         equal_range(const key_t &key)         const { return bin(key).equal_range(key); }
    inline auto         upper_bound(const key_t &key)         const { return bin(key).upper_bound(key); }
    inline auto         lower_bound(const key_t &key)         const { return bin(key).lower_bound(key); }

    inline size_t       size()                                const { return _size; }
    inline size_t       bin_count()                           const { return _data.size(); }
    
    inline void         validate()                                  { _validate(); }
    
   // inline void         test_print(const key_t &key)          const { bin(key).test_print(); }
    
    void test_print(const key_t &key)  const
    {
        auto b = upper_bound(key);
        if (b == end(key)) {
            std::cout << "[END]" << std::endl;
            return;
        }
        std::cout << "[" << (*b).first << " " << (*b).second << "]" << std::endl;
    }

private:

    map_t  _data;
    bool   _valid;
    size_t _size, _category_bits, _mask, _shift;
    bin_t  _empty;

    void _set_bits(size_t category_bits)
    {
        _category_bits = category_bits;
        _shift = KEY_BITS - category_bits;
        _mask = ((ONE << category_bits) - 1) << _shift;
    }

    void _validate()
    {
        if (_valid) return;
        _size = 0;
        for (auto & [key, v] : _data) { v->validate(); _size += v->size();  }
        _valid = true;
        std::cout << "bucket count " << _data.bucket_count() << " load_factor " << _data.load_factor() << std::endl;
    }

    inline key_t _key(const key_t &key) const { return (key & _mask) >> _shift; }

    bin_t *_bin_retrieve_or_create(const key_t &key)
    {
        key_t k(_key(key));
        auto i = _data.find(k);
        if (i != _data.end()) return (*i).second;
        bin_t *b = new bin_t;
        _data.insert(std::pair<key_t, bin_t *>(k, b));
        return b;
    }

    void _clean()
    {
        for (auto & [key, value] : _data) delete value;
        _data.clear();
        _valid = false;
        _size = 0;
    }
};

void test_key_id_index(size_t count);
