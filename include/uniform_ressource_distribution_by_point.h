#ifndef DEF_UNIFORM_RESSOURCE_DISTRIBUTION_BY_POINT_H
#define DEF_UNIFORM_RESSOURCE_DISTRIBUTION_BY_POINT_H

#include "point.h"
#include "uniform_point_distribution.h"

#include <vector>
#include <random>
#include <cstdint>

class uniform_ressource_distribution_by_point
{
    using site_id_type = uint16_t;
    using dimension_type = size_t;
    std::vector<site_id_type> sites_id;
    uniform_point_distribution<dimension_type> point_distribution;

public:
    uniform_ressource_distribution_by_point() = delete;
    uniform_ressource_distribution_by_point(std::vector<site_id_type> sites_id, const dimension_type& width, const dimension_type& height) :
        sites_id{ sites_id.begin(), sites_id.end()},
        point_distribution{0, 0, width, height}{}

    std::pair<point<dimension_type>, site_id_type> operator()(std::default_random_engine& engine) {
        std::uniform_int_distribution<dimension_type> sites_id_distribution{ 0, sites_id.size() - 1 };

        return { point_distribution(engine), sites_id[sites_id_distribution(engine)] };
    }

};
#endif // !DEF_RESSOURCE_GENERATOR_BY_POINTS_H

