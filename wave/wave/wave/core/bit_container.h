#pragma once

template <typename BitContainerCore>
struct bit_container : public BitContainerCore
{
    typedef BitContainerCore      core_t;
    typedef bit_container<core_t> self_t;
    typedef uint64_t              elem_t;

    inline bit_container(bool u = false) : core_t(u) {}
    inline bit_container(const size_t *arr, size_t count) : core_t(arr, count) {}
    inline bit_container(const self_t &u) : core_t(u) {}

    inline self_t           &operator=(const self_t &u)                                 { BitContainerCore::assign(u); return *this; }
    inline self_t           &operator=(bool u)                                          { BitContainerCore::assign(u); return *this; }

    inline bool              operator==(const self_t &u)                          const { return equal(u); }
    inline bool              operator!=(const self_t &u)                          const { return !equal(u); }
    inline self_t            operator&(const self_t &u)                           const { self_t a(*this); op_and(u); return a; }
    inline self_t            operator|(const self_t &u)                           const { self_t a(*this); op_or (u); return a; }
    inline self_t            operator~()                                          const { self_t a(*this); op_not(a); return a; }
    inline self_t           &operator&=(const self_t &u)                                { op_and(u); return *this; }
    inline self_t           &operator|=(const self_t &u)                                { op_or(u);  return *this; }
    inline bool              operator[](size_t i)                                 const { return BitContainerCore::test(i); }

    inline void              set_only(size_t i)                                         { BitContainerCore::assign(false); set(i); }
    inline void              set(size_t i, bool v)                                      { if (v) set(i); else unset(i); }
    inline void              set(const size_t *arr, size_t n, bool v = true)            { for (size_t i = 0; i < n; i++) set(arr[i], v); }
    inline void              unset(const size_t *arr, size_t n)                         { for (size_t i = 0; i < n; i++) unset(arr[i]); }

    inline ssize_t           find_next_set(size_t start = 0, elem_t xor_mask = 0) const { start = bit_find_next_set_bit(this->_set.data(), this->element_count(), start); return start < this->size() ? start : -1; }
    inline ssize_t           find_next_unset(size_t start = 0)                    const { return find_next_set(start, ~elem_t(0)); }
    
 // inline ssize_t           nth_set(size_t n, elem_t xor_mask = 0)               const { return bit_list_nth_set_bit(_set.data(), element_count(), n, 0, xor_mask); }
 // inline ssize_t           nth_unset(size_t n)                                  const { return nth_set(n, ~elem_t(0)); }

 //   inline size_t            list_set_bits(size_t *a, elem_t xor_mask = 0)        const { return bit_list_set_bits(_set.data(), a, size(), 0, xor_mask); }
 //   inline size_t            list_unset_bits(bit_list_t &v)                       const { return list_set_bits(v, ~elem_t(0)); }
};

template <typename BitContainerCore>
std::ostream &print_bit_container(std::ostream &os, const BitContainerCore &o)
{
    typedef typename BitContainerCore::elem_t elem_t;
    bool first = true;
    os << "[" << std::hex;
    const auto *v = o.data();
    for (const auto *e = v + o.element_count(); v != e; ++v) {
        if (!first) os << ".";
        if (*v != 0) os << *v;
        first = false;
    }
    return os << "]" << std::dec;
}


template <typename BitContainerCore>
std::ostream &print_bit_container_binary(std::ostream &os, const BitContainerCore &o)
{
    for (size_t i = 0, sz = o.count(); i < sz; i++)
        os << (o.test(i) ? "1" : "0");
    return os;
}

/*

template <typename BitContainerCore>
inline std::ostream &operator<<(std::ostream &os, const bit_container<BitContainerCore> &o) { return print_bit_container(os, o); }

template <size_t N, typename RNG>
inline size_t shuffled_bit_list(const bit_array<N> &o, typename bit_array<N>::bit_list_t &arr, RNG &rng)
{
    arr = { 0 };
    size_t count = o.list_set_bits(arr);
    std::shuffle(arr.begin(), arr.begin() + count, rng);
    return count;
}

template <size_t N, typename RNG>
inline bit_array<N> unset_random_half_bits(const bit_array<N> &o, RNG &rng)
{
    size_t count = o.count();
    return count < 2 ? o : unset_random_n_bits(o, count / 2, rng);
}

template <size_t N, typename RNG>
inline bit_array<N> unset_random_n_bits(const bit_array<N> &o, size_t n, RNG &rng)
{
    typename bit_array<N>::bit_list_t arr;
    int count = int(shuffled_bit_list(o, arr, rng));
    return bit_array<N>(arr, std::max(count - int(n), 1));
}


template <typename BitContainer, typename RNG>
inline size_t shuffled_bit_list(const BitContainer &o, const size_t *array, size_t count, RNG &rng)
{
    size_t count = o.list_set_bits(array, count);
    std::shuffle(array, array + count, rng);
    return count;
}
 
 */

/*
template <typename BitContainer, typename RNG>
inline BitContainer unset_n_random_bits(const BitContainer &o, size_t n, RNG &rng)
{
    typename bit_array<N>::bit_list_t arr;
    int count = int(shuffled_bit_list(o, arr, rng));
    return BitContainer(arr, std::max(count - int(n), 1));
}

template <typename BitContainer, typename RNG>
inline BitContainer unset_half_random_bits(const BitContainer &o, RNG &rng)
{
    size_t count = o.count();
    return count < 2 ? o : unset_random_n_bits(o, count / 2, rng);
}
*/

/*
inline ssize_t           find_next_set(size_t start = 0, elem_t xor_mask = 0) const { start = bit_find_next_set_bit(_set.data(), ELEM_COUNT, start); return start < N ? start : -1; }
inline ssize_t           find_next_unset(size_t start = 0)                    const { return find_next_set(start, ~elem_t(0)); }

inline ssize_t           nth_set(size_t n, elem_t xor_mask = 0)               const { return bit_list_nth_set_bit(_set.data(), ELEM_COUNT, n, 0, xor_mask); }
inline ssize_t           nth_unset(size_t n)                                  const { return nth_set(n, ~elem_t(0)); }

inline size_t            list_set_bits(bit_list_t &v, elem_t xor_mask = 0)    const { return bit_list_set_bits(_set.data(), ELEM_COUNT, v, 0, xor_mask); }
inline size_t            list_unset_bits(bit_list_t &v)                       const { return list_set_bits(v, ~elem_t(0)); }
*/
