#ifndef DEF_RESSOURCE_GENERATOR_BY_elimination_H
#define DEF_RESSOURCE_GENERATOR_BY_elimination_H

#include <cstdint>
#include <vector>
#include <random>
#include <limits>
class ressource_generator_by_elimination
{
    using site_id_type = uint16_t;
    using nb_of_site_type = uint16_t;
    using dimension_type = size_t;

    std::vector<std::pair<site_id_type, nb_of_site_type>> site_bag;
    dimension_type width;
    dimension_type height;
public:
    ressource_generator_by_elimination() = delete;
    ressource_generator_by_elimination(std::vector<std::pair<site_id_type, nb_of_site_type>> site_bag, const dimension_type& width, const dimension_type& height) :
        site_bag{ site_bag.begin(), site_bag.end() },
        width{ width },
        height{ height } {}


    int operator()(std::default_random_engine& engine)
    {
        std::uniform_int_distribution<> sites_id_distribution{ 0, site_bag.size() - 1 };

        if (site_bag.empty())
        {
            return -1;
        }

        size_t chosen_site = sites_id_distribution(engine);
        auto site = site_bag[chosen_site].first;
        site_bag[chosen_site].second--;
        if (site_bag[chosen_site].second == 0)
        {
            site_bag.erase(site_bag.begin + chosen_site);
        }

        return site;
    }
};
#endif