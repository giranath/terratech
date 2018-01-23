#ifndef DEF_RGB_H
#define DEF_RGB_H

#include <cstdint>

struct rgb
{
    rgb() : r(0), g(0), b(0) {}
    rgb(const uint8_t& r, const uint8_t& g, const uint8_t& b) : r(r), g(g), b(b) {}

    uint8_t r;
    uint8_t g;
    uint8_t b;
};

#endif