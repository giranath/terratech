#ifndef DEF_UNIFORM_POINT_DISTRIBUTION_H
#define DEF_UNIFORM_POINT_DISTRIBUTION_H

#include "point.h"

#include <random>

namespace mapgen
{
template <class T>
class uniform_point_distribution
{
    using dimention_type = T;
    dimention_type max_width;
    dimention_type max_height;
    dimention_type min_width;
    dimention_type min_height;

    std::uniform_int_distribution<dimention_type> distribution_width;
    std::uniform_int_distribution<dimention_type> distribution_height;


public:
    uniform_point_distribution() = delete;

    uniform_point_distribution(dimention_type min_width, dimention_type min_height, dimention_type max_width, dimention_type max_height) :
        distribution_width{ min_width, max_width - 1 },
        distribution_height{ min_height, max_height - 1 } {}

    point<dimention_type> operator()(std::default_random_engine& engine)
    {
        return point<dimention_type>{distribution_width(engine), distribution_height(engine)};
    }
};
}
#endif
