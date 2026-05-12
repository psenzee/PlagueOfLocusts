#include "std_array.h"
#include <cassert>

namespace std_array
{

int test()
{
    // Test: cast
    std::array<float, 3> a1 = {1.1f, 2.2f, 3.3f};
    auto a1_int = std_array::cast<float, int, 3>(a1);
    assert((a1_int == std::array<int, 3>{1, 2, 3}));

    // Test: ints
    std::array<double, 3> a2 = {1.5, 2.5, 3.5};
    auto a2_int = std_array::ints(a2);
    assert((a2_int == std::array<int, 3>{1, 2, 3}));

    // Test: fill
    auto a3 = std_array::fill<int, int, 3>(5);
    assert((a3 == std::array<int, 3>{5, 5, 5}));

    // Test: invert
    std::array<float, 3> a4 = {1.0f, 2.0f, 4.0f};
    auto a4_inv = std_array::invert(a4);
    assert((a4_inv == std::array<float, 3>{1.0f, 0.5f, 0.25f}));

    // Test: negative
    std::array<int, 3> a5 = {1, -2, 3};
    auto a5_neg = std_array::negative(a5);
    assert((a5_neg == std::array<int, 3>{-1, 2, -3}));

    // Test: addition operator
    std::array<int, 3> a6 = {1, 2, 3}, a7 = {4, 5, 6};
    auto sum = a6 + a7;
    assert((sum == std::array<int, 3>{5, 7, 9}));
    
    std::array<int, 3> summed(a6);
    summed += a7;
    assert(sum == summed);
    
    summed += 7;
    assert(summed == (sum + 7));

    // Test: subtraction operator
    auto diff = a6 - a7;
    assert((diff == std::array<int, 3>{-3, -3, -3}));

    // Test: multiplication operator
    auto prod = a6 * a7;
    assert((prod == std::array<int, 3>{4, 10, 18}));

    // Test: divide operator
    auto quotient = a7 / a6;
    assert((quotient == std::array<int, 3>{4, 2, 2}));

    // Test: in-place addition
    std::array<int, 3> a8 = {1, 2, 3};
    std_array::in_place_add(a8, a7);
    assert((a8 == std::array<int, 3>{5, 7, 9}));

    // Test: magnitude
    std::array<int, 3> a9 = {3, 4, 0};
    auto mag = std_array::magnitude(a9);
    assert(mag == 5.0f);

    // Test: dot product
    std::array<int, 3> a10 = {1, 2, 3};
    std::array<int, 3> a11 = {4, 5, 6};
    auto dot_prod = std_array::dot(a10, a11);
    assert(dot_prod == 32);

    // Test: distance
    std::array<int, 3> a12 = {1, 2, 3};
    std::array<int, 3> a13 = {4, 5, 6};
    auto dist = std_array::distance(a12, a13);
    assert(dist - 5.19615f < 0.0001f);

    // Test: rchop (remove last element)
    std::array<int, 4> a14 = {1, 2, 3, 4};
    auto rchopped = std_array::rchop(a14);
    assert((rchopped == std::array<int, 3>{1, 2, 3}));

    // Test: lchop (remove first element)
    auto lchopped = std_array::lchop(a14);
    assert((lchopped == std::array<int, 3>{2, 3, 4}));

    return 0;
}

/*
 template <typename T>             constexpr _AT   normal(const _AT &a)                    { return a / magnitude(a); }
 template <typename T, typename U> constexpr _AT   from(const _AT_1 &a, U last = U(0))     { return _AT({ a[0], a[1], a[2], a[3], a[4], a[5], a[6], T(last) }); }
 template <typename T>             constexpr _AT_1 rchop(const _AT &a)                     { return _AT_1({ a[0], a[1], a[2], a[3], a[4], a[5], a[6] }); }
 template <typename T>             constexpr _AT_1 lchop(const _AT &a)                     { return _AT_1({ a[1], a[2], a[3], a[4], a[5], a[6], a[7] }); }
 template <typename T>             constexpr _AT   lrotate(const _AT &a)                   { return _AT({ a[1], a[2], a[3], a[4], a[5], a[6], a[7], a[0] }); }
 template <typename T>             constexpr _AT   rrotate(const _AT &a)                   { return _AT({ a[7], a[0], a[1], a[2], a[3], a[4], a[5], a[6] }); }
 template <typename T>             constexpr _AT   reorder(const _AT &a, const _ASZ &i)    { return _AT({ a[i[0]], a[i[1]], a[i[2]], a[i[3]], a[i[4]], a[i[5]], a[i[6]], a[i[7]] }); }
 template <typename T>             constexpr _AT   reverse(const _AT &a)                   { return _AT({ a[7], a[6], a[5], a[4], a[3], a[2], a[1], a[0] }); }
 */


}
