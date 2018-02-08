#include "../include/map.h"
#include "../include/uniform_ressource_distribution_by_point.h"
#include "../include/weighted_distribution.h"
#include "../include/elimination_distribution.h"
namespace mapgen
{
map::map(column_type width, row_type height, uint32_t seed) :
    width{ width },
    height{ height },
    engine{ seed },
    biomes_count{ { 0, height * width } },
    regions{ width,std::vector<region>(height) }
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

biome_type map::get_biome_at(const column_type& width, const row_type& height) const
{
    return regions[width][height].get_biome();
}

bool map::has_site_at(const column_type& width, const row_type& height, const site_type& site_id)
{
    return regions[width][height].has_site(site_id);
}
bool map::has_a_site_at(const column_type& width, const row_type& height)
{
    return regions[width][height].has_a_site();
}

void map::generate_by_random_points(const uint16_t& number_of_ressoure, const std::vector<site_type>& ressources_id, const size_t& width, const size_t& height)
{
    uniform_ressource_distribution_by_point distribution(ressources_id, width, height);
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

