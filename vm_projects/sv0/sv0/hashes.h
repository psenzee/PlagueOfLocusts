#pragma once

#include <string>
#include <string_view>

/*
 https://stackoverflow.com/questions/7666509/hash-function-for-string
 https://burtleburtle.net/bob/hash/evahash.html
 https://cp-algorithms.com/string/rabin-karp.html
 https://cp-algorithms.com/string/string-hashing.html
 https://en.wikipedia.org/wiki/MurmurHash
 */

/*
inline uint64_t hash_index(uint64_t hash)
{
    // https://probablydance.com/2018/06/16/fibonacci-hashing-the-optimization-that-the-world-forgot-or-a-better-alternative-to-integer-modulo/
    // we want to avoid % (of non power-of-two)
    hash ^= hash >> shift_amount;
    return (11400714819323198485LLU * hash) >> shift_amount;
}
 */

template <typename IterT>
inline uint64_t calculate_hash(IterT i, IterT e)
{
    size_t hash = 5381;
    for (; i != e; ++i) {
        hash = ((hash << 5) + hash) + *i; /* hash * 33 + c */
    }
    return hash;
}

/*
int hash(char const *input)
{
    int result = 0x55555555;

    while (*input) {
        result ^= *input++;
        result = rol(result, 5);
    }
}*/

template <typename CharT>
inline uint64_t calculate_hash(const CharT *s) // c-style without an implict strlen (which string_view would need)
{
    size_t hash = 5381;
    for (int c; (c = *s); ++s) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

template <typename CharT>
inline uint64_t calculate_hash(std::basic_string_view<CharT> sv)
{
    return calculate_hash(sv.begin(), sv.end());
}

template <typename CharT>
inline uint64_t calculate_hash(const std::basic_string<CharT> &s)
{
    return calculate_hash(s.begin(), s.end());
}
