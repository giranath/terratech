#ifndef DEF_PPM_H
#define DEF_PPM_H

#include <vector>
#include <iostream>

#include "rgb.h"

struct ppm
{
    ppm(const std::vector<std::vector<rgb>>& data) 
    : data(data)
    , heigth(data.front().size())
    , width(data.size()) {}

    size_t width;
    size_t heigth;
    std::vector < std::vector<rgb>> data;

    std::ostream& write(std::ostream& out) const
    {
        out << "P3\n";
        out << width;
        out << " ";
        out << heigth;
        out << "\n255\n";

	for(std::size_t y = 0; y < heigth; ++y) {
		for(std::size_t x = 0; x < width; ++x) {
			out << data[x][y] << " ";
		}
		out << "\n";
	}

	return out;
    }
};

std::ostream& operator<<(std::ostream& out, const ppm& image) {
	return image.write(out);
}
#endif
