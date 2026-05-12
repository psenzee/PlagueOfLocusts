#pragma once

#include <cstddef>

namespace primes
{

const unsigned *primes_begin();
const unsigned *primes_end();
size_t          primes_count();
unsigned        prime_nth(unsigned index);

const unsigned *prime_search(unsigned value);
unsigned        prime_equal_or_larger(unsigned value);
unsigned        prime_equal_or_smaller(unsigned value);
unsigned        prime_next_larger(unsigned value);
unsigned        prime_next_smaller(unsigned value);

}
