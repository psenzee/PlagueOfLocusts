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

    inline size_t us_elapsed() const { return std::chrono::duration_cast<std::chrono::microseconds>(clock_t::now() - _start).count(); }

    std::ostream &print_elapsed(std::ostream &os, const char *message = 0) const
    {
        if (message && *message) os << message << " ";
        return print_elapsed_only(os << "elapsed time: ") << std::endl;
    }

    inline std::ostream &print_elapsed_only(std::ostream &os) const
    {
        return write(os, us_elapsed());
    }
    
    static std::ostream &write(std::ostream &os, size_t us)
    {
        double ms = us / 1000.0, secs = us / 1000000.0;
        return os << std::dec << std::fixed << std::setprecision(3) << secs << "s (" << std::setprecision(1) << ms << "ms " << us << "\u03bcs)";
    }

    inline std::ostream &print_elapsed(const char *message = 0) const { return print_elapsed(std::cout, message); }
};
