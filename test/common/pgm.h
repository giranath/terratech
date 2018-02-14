#ifndef DEF_PGM_H
#define DEF_PGM_H

#include <cstdint>
#include <vector>
#include <iostream>

class pgm {
    size_t width;
    size_t height;
    std::vector<std::vector<uint8_t>> data;

public:
    pgm(const std::vector<std::vector<uint8_t>> &data)
    : width(data.size())
    , height(data.front().size())
    , data(data) {
    }

    std::ostream &write(std::ostream &out) const {
        // Write header
        out << "P2\n";
        out << width;
        out << " ";
        out << height;
        out << "\n255\n";

        for (std::size_t y = 0; y < height; ++y) {
            for (std::size_t x = 0; x < width; ++x) {
                out << static_cast<int>(data[x][y]) << " ";
            }
            out << "\n";
        }

        return out;
    }
};

std::ostream &operator<<(std::ostream &out, const pgm &image) {
    return image.write(out);
}


#endif
