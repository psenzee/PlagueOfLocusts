#pragma once

#include <iostream>
#include <iomanip>
#include <chrono>

class simple_timer
{
    typedef std::chrono::high_resolution_clock clock_t;
    std::chrono::time_point<clock_t> _start;

public:

    inline simple_timer() : _start(clock_t::now()) {}

    inline auto us_elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(clock_t::now() - _start); }
    
    std::ostream &print_elapsed(std::ostream &os, const char *message = 0) const
    {
        auto duration = us_elapsed();
        double secs = duration.count() / 1000000.0;
        if (message && *message) std::cout << message << " ";
        return os << "elapsed time: " << std::dec << std::fixed << std::setprecision(5) << secs << "s (" << duration.count() << " microseconds)" << std::endl;
    }
    
    inline std::ostream &print_elapsed(const char *message = 0) const { return print_elapsed(std::cout, message); }
};
