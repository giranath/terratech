#ifndef DEF_CLUMP_H
#define DEF_CLUMP_H

#include "point.h"

#include <cstdint>
#include <vector>
#include <random>
#include <algorithm>
namespace mapgen
{

class clump
{
    int min_size;
    int max_size;
    std::vector<point<int>> points;
    std::default_random_engine engine;

    void add_random_point_to_clump(const point<int>& p)
    {
        std::uniform_int_distribution<> distribution{ -1,1 };
        auto it = points.begin();
        point<int> point_to_add;

        while(it != points.end())
        {
            point_to_add = point<int>{ p.x + distribution(engine), p.x + distribution(engine) };
            it = std::find(points.begin(), points.end(), point_to_add);
        }
        points.emplace_back(point_to_add);
    }

public:

    clump() : min_size{ 1 }, max_size{ 1 }, engine{}, points(1) {}
    
    clump(int min_size, int max_size, uint32_t seed) : min_size{ min_size }, max_size{ max_size }, engine{ seed }, points{} {}

    std::vector<point<int>> operator()() 
    {
        points.clear();
        point<int> point_to_add_from{};
        points.push_back(point_to_add_from);

        std::uniform_int_distribution<> dis{ min_size, max_size };
        uint8_t nb = dis(engine);

        for(uint8_t i = 0; i < nb; ++i)
        {
            add_random_point_to_clump(point_to_add_from);
            std::uniform_int_distribution<> distance_distribution(0, points.size() - 1);
            point_to_add_from = points[distance_distribution(engine)];
        }
        return points;
    }
};
}
#endif