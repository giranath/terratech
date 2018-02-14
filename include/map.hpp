#ifndef DEF_MAP_H
#define DEF_MAP_H

#include "region.hpp"
#include "map_type.hpp"
#include "probability_structure.hpp"
#include "biome_table.hpp"
#include "perlin_noise.hpp"

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

    mapgen::biome_table biome_tab;
    mapgen::biome_axis altitude_tab;

    mapgen::perlin_noise temperature_noise;
    mapgen::perlin_noise humidity_noise;
    mapgen::perlin_noise altitude_noise;
public:

    map(column_type width, row_type height, uint32_t seed);
    map(column_type width, row_type height, uint32_t seed, const mapgen::biome_table& biome_tab);
    map(column_type width, row_type height, uint32_t seed, const mapgen::biome_table& biome_tab, mapgen::biome_axis altitude_tab);

    void add_site_by_noise(const double& threshold, const site_type& site_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise);
    void set_biome_by_noise(const double& threshold, const biome_type& biome_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise);

    void set_biome_at(const column_type& x, const column_type& y, const biome_type& biome_id);

    void set_biome_with_table();
    biome_type get_biome_at(const column_type& x, const row_type& y) const;

    void add_site_at(const column_type& x, const column_type& y, const site_type& site_id);
    bool has_site_at(const column_type& x, const row_type& y, const site_type& site_id);
    bool has_a_site_at(const column_type& x, const row_type& y);
    std::vector<site_type> get_sites_at(const column_type& x, const row_type& y);

    void generate_by_random_points(const uint16_t& number_of_ressoure, const std::vector<site_type>& ressources_id, const size_t& width, const size_t& height);
    void generate_by_weight_and_biome(probability_structure<double>& biome_weighted_struct);
    void generate_by_elimination(probability_structure<int16_t>& site_bag);
};
}
#endif