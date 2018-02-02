#include "../include/map.h"

map::map(column_type width, row_type heigth) : regions(width, std::vector<region>(height, region())), width(width), height(heigth)
{}

void map::add_site_by_noise(const double& threshold, const uint16_t& site_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise)
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
void map::set_biome_by_noise(const double& threshold, const uint16_t& biome_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise)
{
    for (column_type i = 0; i < width; ++i)
    {
        for (row_type j = 0; j < height; ++j)
        {
            if (op(noise[i][j], threshold))
            {
                regions[i][j].set_biome(biome_id);
            }
        }
    }
}

region map::get_region(column_type i, row_type j) const
{
    return regions[i][j];
}