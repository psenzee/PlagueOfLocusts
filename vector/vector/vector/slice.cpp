#include "slice.h"

#include <iostream>
#include <cmath>
#include <vector>
#include <cstddef>

#include "simple_timer.h"
#include "generators.h"

void test_slice(size_t count)
{
    typedef float float_t;
    simple_timer t;
    std::vector<float_t> nums;
    std::cout << "generating " << count << " items" << std::endl;
    simple_timer ta;
    real_generator<float_t> reals;
    reals.vector(count, nums);
    ta.print_elapsed("  slice generate");
    auto sli = slice<float_t, uint32_t>();
    simple_timer tb;
    for (auto i = nums.begin(), e = nums.end(); i != e; ++i)
        sli.add_invalidate(*i);
    tb.print_elapsed("  slice invalidating add");
    simple_timer tc;
    sli.index();
    tc.print_elapsed("  slice index");
    t.print_elapsed("slice total");
}
