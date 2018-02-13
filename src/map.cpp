#include "../include/map.hpp"
#include "../include/uniform_ressource_distribution_by_point.hpp"
#include "../include/weighted_distribution.hpp"
#include "../include/elimination_distribution.hpp"

#include <cmath>

namespace mapgen
{
map::map(column_type width, row_type height, uint32_t seed) :
    width{ width },
    height{ height },
    regions{ width,std::vector<region>(height) },
    biomes_count{ { 0, height * width } },
    engine{ seed }
{
}

map::map(column_type width, row_type height, uint32_t seed, const mapgen::biome_table& biome_tab):
    width{ width },
    height{ height },
    regions{ width,std::vector<region>(height) },
    biomes_count{ { 0, height * width } },
    engine{ seed },
    biome_tab{ biome_tab },
    temperature_noise{seed},
    humidity_noise{ seed + 1},
    altitude_noise{ seed + 2 }
{
}
map::map(column_type width, row_type height, uint32_t seed, const mapgen::biome_table& biome_tab, mapgen::biome_axis altitude_tab) :
    width{ width },
    height{ height },
    regions{ width,std::vector<region>(height) },
    biomes_count{ { 0, height * width } },
    engine{ seed },
    biome_tab{ biome_tab },
    altitude_tab{ altitude_tab },
    temperature_noise{ seed },
    humidity_noise{seed +1},
    altitude_noise{seed + 2}
{
}

void map::add_site_by_noise(const double& threshold, const site_type& site_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise)
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
void map::set_biome_by_noise(const double& threshold, const biome_type& biome_id, std::function<bool(double, double)> op, const std::vector<std::vector<double>>& noise)
{
    for (column_type i = 0; i < width; ++i)
    {
        for (row_type j = 0; j < height; ++j)
        {
            if (op(noise[i][j], threshold))
            {
                set_biome_at(i, j, biome_id);
            }
        }
    }
}

void map::add_site_at(const column_type& x, const column_type& y, const site_type& site_id)
{
    regions[x][y].add_site(site_id);
}

void map::set_biome_at(const column_type& x, const column_type& y, const biome_type& biome_id)
{
    biome_type previous_biome = regions[x][y].get_biome();
    biomes_count[previous_biome] = --biomes_count[previous_biome];

    regions[x][y].set_biome(biome_id);
    biomes_count[biome_id] = ++biomes_count[biome_id];
}

void map::set_biome_with_table()
{
    for (column_type i = 0; i < width; ++i)
    {
        for (row_type j = 0; j < height; ++j)
        {
            double altitude = altitude_noise.octave_noise(static_cast<double>(i) / width, static_cast<double>(j) / height, 0.0, 8, 0.5);
            biome_type biome_altitude = altitude_tab.biome_with(std::abs(altitude));
            if (biome_altitude == mapgen::biome_axis::NO_SPECIAL_BIOME) {
                double col = temperature_noise.octave_noise(static_cast<double>(i) / width, static_cast<double>(j) / height, 0.0, 8, 0.5);
                double row = humidity_noise.octave_noise(static_cast<double>(i) / width, static_cast<double>(j) / height, 0.0, 8, 0.5);
                set_biome_at(i, j, biome_tab.biome_with(col, row));
            }
            else
            {
                set_biome_at(i, j, biome_altitude);
            }
        }
    }
}

biome_type map::get_biome_at(const column_type& x, const row_type& y) const
{
    return regions[x][y].get_biome();
}

bool map::has_site_at(const column_type& x, const row_type& y, const site_type& site_id)
{
    return regions[x][y].has_site(site_id);
}
bool map::has_a_site_at(const column_type& x, const row_type& y)
{
    return regions[x][y].has_a_site();
}


std::vector<site_type> map::get_sites_at(const column_type& x, const row_type& y)
{
    return regions[x][y].get_sites();
}

void map::generate_by_random_points(const uint16_t& number_of_ressoure, const std::vector<site_type>& ressources_id, const size_t& width_size, const size_t& height_size)
{
    uniform_ressource_distribution_by_point distribution(ressources_id, width_size, height_size);
    for (uint16_t i = 0; i < number_of_ressoure; ++i)
    {
        std::pair<point<size_t>, uint16_t> p = distribution(engine);
        regions[p.first.x][p.first.y].add_site(p.second);
    }
}
void map::generate_by_weight_and_biome(probability_structure<double>& biome_weighted_struct)
{
    std::vector<std::pair<biome_type, weighted_distribution<site_type>>> distributions;
    distributions.reserve(biome_weighted_struct.size() - 1);

    for (auto& v : biome_weighted_struct.get_all_biomes())
    {
        distributions.push_back({ v, weighted_distribution<site_type>{biome_weighted_struct.get_site_occurence(v)} });
    }
    for (auto& row : regions)
    {
        for (auto& val : row)
        {
            auto it = std::find_if(distributions.begin(), distributions.end(), [&val](std::pair<biome_type, weighted_distribution<site_type>>& dis) {
                return val.get_biome() == dis.first;
            });
            if (it != distributions.end())
            {
                val.add_site(distributions[val.get_biome()].second(engine));
            }
        }
    }
}
void map::generate_by_elimination(probability_structure<int16_t>& site_bag)
{
    std::vector<std::pair<biome_type, elimination_distribution<site_type>>> distributions;
    uniform_point_distribution<size_t> point_distribution{ 0, 0, regions.size() - 1, regions.front().size() - 1 };
    distributions.reserve(site_bag.size() - 1);

    for (auto& v : site_bag.get_all_biomes())
    {
        distributions.push_back({ v, elimination_distribution<site_type>{site_bag.get_site_occurence(v)} });
    }

    point<size_t> p = point_distribution(engine);
    std::map<biome_type, unsigned int> count;
    while (!distributions.empty())
    {

        uint16_t biome = regions[p.x][p.y].get_biome();
        auto it = find_if(distributions.begin(), distributions.end(), [&biome](std::pair<biome_type, elimination_distribution<site_type>>& value) {
            return biome == value.first;
        });

        if (it != distributions.end() && !it->second.empty())
        {
            regions[p.x][p.y].add_site(it->second(engine));
            ++count[biome];
        }

        else if (it != distributions.end() && (it->second.empty() || count[biome] >= biomes_count[biome]))
        {
            distributions.erase(it);
        }

        p = point_distribution(engine);
    }
}
}

