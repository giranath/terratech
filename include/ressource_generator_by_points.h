#ifndef DEF_RESSOURCE_GENERATOR_BY_POINTS_H
#define DEF_RESSOURCE_GENERATOR_BY_POINTS_H

#include "point.h"

#include <vector>
#include <random>
#include <cstdint>

class ressource_generator_by_points
{
    using site_id_type = uint16_t;
    using dimension_type = size_t;
    std::vector<site_id_type> sites_id;
    dimension_type width;
    dimension_type height;

public:
    ressource_generator_by_points() = delete;
    ressource_generator_by_points(std::vector<site_id_type> sites_id, dimension_type width, dimension_type height) :
        sites_id{ sites_id.begin(), sites_id.end()},
        width{width},
        height{height}{}

    std::pair<point<dimension_type>, site_id_type> operator()(std::default_random_engine& engine) {
        std::uniform_int_distribution<> width_distribution{0, width};
        std::uniform_int_distribution<> height_distribution{ 0, height };
        std::uniform_int_distribution<> sites_id_distribution{ 0, sites_id.size() - 1 };

        return { point<dimension_type>{ width_distribution(engine), height_distribution(engine)}, sites_id[sites_id_distribution(engine)] };
    }

};
#endif // !DEF_RESSOURCE_GENERATOR_BY_POINTS_H

