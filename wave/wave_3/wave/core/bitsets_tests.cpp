#include "bitsets.h"

template <std::size_t N>
void bitset_test(const std::bitset<N> &bs0)
{
    bitset_print(std::cout, bs0) << "\n";
    auto list0 = list_from_bitset(bs0);
    bit_list_print(std::cout, list0) << "                  count " << bit_list_count(list0) << "\n";
    auto bs1 = bitset_from_list(list0);
    bitset_print(std::cout, bs1) << "\n";
    auto list1 = list_from_bitset(bs1);
    bit_list_print(std::cout, list1) << "                  count " << bit_list_count(list1) << "\n";
    std::mt19937 mt(N);
    auto list2(list1);
    bit_list_shuffle(list2, mt);
    bit_list_print(std::cout, list2) << " (shuffled)       count " << bit_list_count(list2) << "\n";
    auto bs2 = bitset_from_list(list2);
    bitset_print(std::cout, bs2) << "\n";
    auto list3(list1);
    bit_list_shuffle(list3, mt);
    bit_list_print(std::cout, list3) << " (shuffled)       count " << bit_list_count(list3) << "\n";
    auto list4(list3);
    bit_list_insert(list4, 51);
    bit_list_insert(list4, 46);
    bit_list_insert(list4, 25);
    bit_list_print(std::cout, list4) << " (dupes inserted) count " << bit_list_count(list4) << "\n";
    bit_list_normal_form(list4);
    bit_list_print(std::cout, list4) << " (normal form)    count " << bit_list_count(list4) << "\n";
}

int bitset_test_main()
{
    std::mt19937 mt(100);
    auto bs0 = bitset_init_random<64>(mt);
    bitset_test(bs0);
    bitset_test(std::bitset<64>());
    std::bitset<64> m(false);
    m[0] = true;
    bitset_test(m);
    return 0;
}
