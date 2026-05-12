#include "stl_tests.h"
#include "stl.h"
#include "arrays.h"

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
    auto value = arrays::dot(a, b);
    auto valuea = arrays::dot(a);
    auto valuev = arrays::dot(b);
}
