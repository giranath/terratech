#include "../include/map.h"

Map::Map(column_type width, row_type heigth) : regions(width, std::vector<Region>(height, Region())), width(width), height(heigth) {}

void Map::add_site_by_noise(const double& threshold, const uint16_t& site_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise)
{
    for (column_type i = 0; i < width; ++i)
    {
        for (row_type j = 0; j < height; ++j)
        {
            if (op(noise[i][j], threshold))
            {
                regions[i][j].add_site(site_id);
            }
        }
    }
}

Region Map::get_region(column_type i, row_type j) const
{
    return regions[i][j];
}