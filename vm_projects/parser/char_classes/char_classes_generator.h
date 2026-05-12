#pragma once

#include <cstdint>
#include <array>
#include <iostream>

struct char_classes_generator
{
    static const uint32_t SIZE = 1 << 8;

    typedef std::array<uint32_t, SIZE> array_t;

    std::ostream &generate(std::ostream &os);

private:

    static void initialize();

    static const array_t &data() { return _data; }

    static std::ostream &dump(std::ostream &os, const char *class_name);

    static constexpr uint32_t _classes(int ch);

    static array_t _data;
};
