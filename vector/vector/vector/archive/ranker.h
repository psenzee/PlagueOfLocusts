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

#include "stl.h"
#include "rank.h"

struct rank_index_encoder_null
{
    inline int32_t encode(size_t v)  const { return int32_t(v); }
    inline size_t  decode(int32_t v) const { return size_t(v); }
};

template <int N, typename FloatT, typename SignedIndexT = int16_t, typename IndexEncoderT = rank_index_encoder_null>
struct ranker
{
    enum { COUNT = N };

    typedef FloatT                                      value_type;
    typedef SignedIndexT                                signed_index_t;
    typedef std::pair<value_type, SignedIndexT>         pair_t;
    typedef rank<N, FloatT, SignedIndexT>               rank_t;
    typedef std::vector<pair_t>                         ranked_vector_t;
    typedef std::vector<value_type>                     vector_t;
    
    const IndexEncoderT &_encoder;
    ranked_vector_t _data;
    
    ranker(const IndexEncoderT &encoder = IndexEncoderT()) : _encoder(encoder) {}
    
    rank_t rank(const vector_t &v)
    {
        if (_data.size() < v.size()) _data.resize(v.size());
        size_t i = 0;
        for (auto e : v) { _data[i] = pair_t(e, i); i++; }
        // do a partial sort, descending by absolute value
        std::nth_element(_data.begin(), _data.begin() + N, _data.end(), compare_0ad_1u<pair_t>());
        return rank_t(_data);
    }
};

void test_ranker_1 (size_t dims, size_t count);
void test_ranker_2 (size_t dims, size_t count);
void test_ranker_3 (size_t dims, size_t count);
void test_ranker_4 (size_t dims, size_t count);
void test_ranker_8 (size_t dims, size_t count);
void test_ranker_12(size_t dims, size_t count);
void test_ranker_16(size_t dims, size_t count);
void test_ranker_32(size_t dims, size_t count);
