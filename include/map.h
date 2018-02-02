#ifndef DEF_MAP_H
#define DEF_MAP_H

#include "region.h"
#include "ressource_generator.h"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <functional>

class map
{
    using column_type = std::size_t;
    using row_type = std::size_t;

    column_type width;
    row_type height;
    

public:
    std::vector<std::vector<region>> regions;

    map(column_type width, row_type heigth);

    void add_site_by_noise(const double& threshold, const uint16_t& site_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise);
    void set_biome_by_noise(const double& threshold, const uint16_t& biome_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise);
    region get_region(column_type i, row_type j) const;
};

#endif