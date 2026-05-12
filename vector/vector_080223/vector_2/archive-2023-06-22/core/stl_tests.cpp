#include "stl_tests.h"
#include "stl.h"

#include <vector>
#include <array>
#include <iostream>

void test_count_unique_on_sorted()
{
    std::array<int, 12> a = { 0, 1, 2, 3, 4, 0, 6, 7, 8, 9, 3, 4 };
    std::sort(a.begin(), a.end(), std::less<int>());
    auto count = count_unique_on_sorted(a.begin(), a.end(), std::less<int>());
    std::cout << " count " << count << std::endl;
    std::vector<int> b;
    count = count_unique_on_sorted(b.begin(), b.end(), std::less<int>());
    std::cout << " count " << count << std::endl;
    b.push_back(2); b.push_back(2);
    count = count_unique_on_sorted(b.begin(), b.end(), std::less<int>());
    std::cout << " count " << count << std::endl;
}

void test_math()
{
    std::array<float_t, 12> a = { 0 };
    std::vector<double> b;
    for (size_t i = 0; i < 12; i++) {
        b.push_back(i);
        a[i] = i;
    }
    auto value = dot(a, b);
    auto valuea = magnitude_sq(a);
    auto valuev = magnitude_sq(b);
    
    auto dist1 = lp_distance_power<1>(a, b), dist2 = lp_distance_power<2>(a, b), dist3 = lp_distance_power<3>(a, b);
    dist1 = lp_distance<1>(a, b);
    dist2 = lp_distance<2>(a, b);
    dist3 = lp_distance<3>(a, b);
}
