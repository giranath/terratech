#include "../include/map.hpp"

namespace mapgen {

map::map(column_type width, row_type heigth)
: width(width)
, height(heigth)
, regions(width, std::vector<region>(height, region())){}

void map::add_site_by_noise(const double &threshold, const uint16_t &site_id, predicate pred,
                            const std::vector <std::vector<double>> &noise) {
    for (column_type i = 0; i < width; ++i) {
        for (row_type j = 0; j < height; ++j) {
            if (pred(noise[i][j], threshold)) {
                regions[i][j].add_site(site_id);
            }
        }
    }
}

region map::get_region(column_type i, row_type j) const {
    return regions[i][j];
}
}