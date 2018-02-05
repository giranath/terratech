#ifndef DEF_MAPGEN_MAP_HPP
#define DEF_MAPGEN_MAP_HPP

#include "region.hpp"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <functional>

namespace mapgen {

class map {
    using column_type = std::size_t;
    using row_type = std::size_t;
    using predicate = std::function<bool(double, double)>;

    column_type width;
    row_type height;
    std::vector<std::vector<region>> regions;

public:
    map(column_type width, row_type heigth);

    void add_site_by_noise(const double &threshold, const uint16_t &site_id, predicate pred, const std::vector<std::vector<double>>& noise);
    region get_region(column_type i, row_type j) const;
};

}

#endif