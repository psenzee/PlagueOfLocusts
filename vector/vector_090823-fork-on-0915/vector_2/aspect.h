#pragma once

#include <iostream>

#include "stl.h"
#include "compile_time.h"

template <typename FloatT, size_t N, typename IdT, bool SortShapeBool>
struct aspect
{
    static constexpr bool   SORT_SHAPE      = SortShapeBool;
    static constexpr size_t DIMENSION_COUNT = N,
                            SHAPE_COUNT     = compile_time::isqrt(N),
                            SHAPE_BITS      = compile_time::lg(SHAPE_COUNT);

    typedef FloatT                                                real_t;
    typedef IdT                                                   id_t;
    typedef typename int_for_max_value<DIMENSION_COUNT>::signed_t dimension_t;
    typedef typename int_for_max_value<SHAPE_COUNT>::unsigned_t   shape_t;
    typedef aspect<real_t, DIMENSION_COUNT, id_t, SORT_SHAPE>     self_t;

    id_t        id;
    real_t      norm;
    dimension_t dimension;
    shape_t     shape;

    inline aspect(dimension_t dimension_, shape_t shape_, real_t norm_, id_t id_ = ~id_t(0)) : dimension(dimension_), shape(shape_), norm(norm_), id(id_) {}
    inline aspect(const self_t &u) : dimension(u.dimension), norm(u.norm), shape(u.shape), id(u.id) {}
    
    struct compare_norm                 { inline bool operator()(const self_t &a, const self_t &b) const { return a.norm < b.norm; } };
    struct compare_dimension            { inline bool operator()(const self_t &a, const self_t &b) const { return a.dimension < b.dimension; } };
    struct compare_shape                { inline bool operator()(const self_t &a, const self_t &b) const { return _compare_shape(a, b); } };
    struct compare_shape_norm           { inline bool operator()(const self_t &a, const self_t &b) const { return _compare_shape_norm(a, b); } };
    struct compare_dimension_shape      { inline bool operator()(const self_t &a, const self_t &b) const { return a.dimension != b.dimension ? a.dimension < b.dimension : _compare_shape(a, b); } };
    struct compare_dimension_shape_norm { inline bool operator()(const self_t &a, const self_t &b) const { return a.dimension != b.dimension ? a.dimension < b.dimension : _compare_shape_norm(a, b); } };
    
    inline bool operator<(const self_t &u) const { return dimension != u.dimension ? dimension < u.dimension : (SORT_SHAPE && shape != u.shape ? shape < u.shape : (norm < u.norm)); }

private:
    
    inline static bool _compare_shape(const self_t &a, const self_t &b)      { return SORT_SHAPE && a.shape < b.shape; }
    inline static bool _compare_shape_norm(const self_t &a, const self_t &b) { return SORT_SHAPE && a.shape != b.shape ? a.shape < b.shape : (a.norm < b.norm); }
};

template <typename FloatT, size_t N, typename IdT, bool SortShapeBool>
std::ostream &operator<<(std::ostream &os, const aspect<FloatT, N, IdT, SortShapeBool> &u) { return os << "[d " << u.dimension << std::setprecision(2) << " s " << u.shape << " n " << u.norm << " id " << u.id << "]"; }
