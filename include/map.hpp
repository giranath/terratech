#ifndef DEF_MAP_H
#define DEF_MAP_H

#include "region.hpp"
#include "map_type.hpp"
#include "probability_structure.hpp"

#include <vector>
#include <algorithm>
#include <cstdint>
#include <functional>
#include <map>
#include <random>
namespace mapgen
{
class map
{
    column_type width;
    row_type height;

    std::vector<std::vector<region>> regions;
    std::map<biome_type, unsigned int> biomes_count;
    std::default_random_engine engine;
public:

    map(column_type width, row_type heigth, uint32_t seed);

    void add_site_by_noise(const double& threshold, const site_type& site_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise);
    void set_biome_by_noise(const double& threshold, const biome_type& biome_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise);

    void set_biome_at(const column_type& x, const column_type& y, const biome_type& biome_id);
    biome_type get_biome_at(const column_type& width, const row_type& height) const;

    void add_site_at(const column_type& x, const column_type& y, const site_type& site_id);
    bool has_site_at(const column_type& width, const row_type& height, const site_type& site_id);
    bool has_a_site_at(const column_type& width, const row_type& height);

    void generate_by_random_points(const uint16_t& number_of_ressoure, const std::vector<site_type>& ressources_id, const size_t& width, const size_t& height);
    void generate_by_weight_and_biome(probability_structure<double>& biome_weighted_struct);
    void generate_by_elimination(probability_structure<int16_t>& site_bag);
};
}
#endif