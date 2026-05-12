#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <cmath>
#include "key.h"

template <typename FloatT, size_t N, size_t KeyRanksN, typename IdT = uint32_t>
struct key_id
{
    typedef FloatT                       real_t;
    typedef key<FloatT, N, KeyRanksN>    key_t;
    typedef key_id<FloatT, N, KeyRanksN> self_t;
    typedef IdT                          id_t;

    key_t  key;
    id_t   id;

    inline key_id(const key_t &key, id_t id) : key(key), id(id) {}
    inline key_id() : key(), id(0) {}

    struct sort_comparer       { inline bool operator()(const self_t &a, const self_t &b) const { int r = key_t::compare(a.key, b.key); return r ? r < 0 : a.id < b.id; } };
    struct exp_norm_comparer   { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_norm(a.key, b.key) < 0; } };
    struct norm_comparer       { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__norm(a.key, b.key) < 0; } };
    struct initial_comparer    { inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__initial(a.key, b.key) < 0; } };

    struct rank_comparer {
        size_t _rank;
        inline rank_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__rank(_rank, a.key, b.key) < 0; }
    };

    struct value_comparer {
        size_t _rank;
        inline value_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__value(_rank, a.key, b.key) < 0; }
    };

    struct exp_value_comparer {
        size_t _rank;
        inline exp_value_comparer(size_t r) : _rank(r) {}
        inline bool operator()(const self_t &a, const self_t &b) const { return key_t::compare__exp_value(_rank, a.key, b.key) < 0; }
    };
};

template <typename FloatT, size_t N, size_t KeyRanksN, typename IdT>
inline std::ostream &operator<<(std::ostream &os, const key_id<FloatT, N, KeyRanksN, IdT> &key) { return os << key.key << ":" << key.id; }
