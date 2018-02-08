#ifndef DEF_PPM_H
#define DEF_PPM_H

#include <vector>
#include <iostream>

#include "rgb.h"

class ppm {
    size_t width;
    size_t height;
    std::vector<std::vector<rgb>> data;

public:
    ppm(const std::vector<std::vector<rgb>> &data)
    : width(data.size())
    , height(data.front().size())
    , data(data) {

    }

    std::ostream& write(std::ostream &out) const {
        out << "P3\n";
        out << width;
        out << " ";
        out << height;
        out << "\n255\n";

        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                out << data[x][y] << " ";
            }
            out << "\n";
        }

        return out;
    }
};

std::ostream &operator<<(std::ostream &out, const ppm &image) {
    return image.write(out);
}

#endif
