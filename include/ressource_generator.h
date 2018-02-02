#ifndef DEF_RESSOURCE_GENERATOR_H
#define DEF_RESSOURCE_GENERATOR_H

#include "uniform_ressource_distribution_by_point.h"
#include "weighted_distribution.h"
#include "elimination_distribution.h"
#include "point.h"
#include "region.h"

#include <cstdint>
#include <vector>
#include <random>
#include <algorithm>
class ressource_generator
{
    std::vector<std::vector<region>>& regions;
    std::default_random_engine engine;
public:
    ressource_generator(std::vector<std::vector<region>>& regions) : regions{ regions } {}
    void generate_by_random_points(const uint16_t& number_of_ressoure, const std::vector<uint16_t>& ressources_id, const size_t& width, const size_t& height)
    {
        uniform_ressource_distribution_by_point distribution(ressources_id, width, height);
        for (uint16_t i = 0; i < number_of_ressoure; ++i)
        {
            std::pair<point<size_t>, uint16_t> p = distribution(engine);
            regions[p.first.x][p.first.y].add_site(p.second);
        }
    }

    void generate_by_weight_and_biome( const std::vector<std::pair<uint16_t, std::vector<std::pair<uint16_t, double>>>>& biome_weighted_struct)
    {
        std::vector<std::pair<uint16_t, weighted_distribution<uint16_t>>> distributions;
        distributions.reserve(biome_weighted_struct.size() - 1);

        for (auto& v : biome_weighted_struct)
        {
            distributions.push_back({v.first, weighted_distribution<uint16_t>{v.second} });
        }

        for (auto& row : regions)
        {
            for(auto& val : row)
            {
                auto it = std::find_if(distributions.begin(), distributions.end(), [&val](std::pair<uint16_t, weighted_distribution<uint16_t>>& dis) {
                    return val.get_biome() == dis.first;
                });
                if (it != distributions.end())
                {
                    val.add_site(distributions[val.get_biome()].second(engine));
                }
            }
        }
    }

    void generate_by_elimination(std::vector<std::pair<uint16_t, int16_t>> site_bag)
    {
        elimination_distribution<uint16_t> distribution(site_bag);
        uniform_point_distribution<size_t> point_distribution(0, 0, regions.size() -1, regions.front().size() -1);

        while(!distribution.empty())
        {
            point<size_t> p = point_distribution(engine);
            uint16_t site_id = distribution(engine);
            regions[p.x][p.y].add_site(site_id);
        }
    }
};

#endif
