#ifndef DEF_PGM_H
#define DEF_PGM_H

#include <cstdint>
#include <vector>
#include <iostream>

struct pgm
{
    pgm(const std::vector < std::vector<uint8_t>>& data) 
    : data(data)
    , heigth(data.front().size())
    , width(data.size()) {
    }

    size_t width;
    size_t heigth;
    std::vector < std::vector<uint8_t>> data;

    std::ostream& write(std::ostream& out) const
    {
	// Write header
        out << "P2\n";
        out << width;
        out << " ";
        out << heigth;
        out << "\n255\n";

	for(std::size_t y = 0; y < heigth; ++y) {
		for(std::size_t x  = 0; x < width; ++x) {
			out << static_cast<int>(data[x][y]) << " ";
		}
		out << "\n";
	}

	return out;
    }
};

std::ostream& operator<<(std::ostream& out, const pgm& image) {
	return image.write(out);
}



#endif
