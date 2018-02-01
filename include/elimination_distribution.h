#ifndef DEF_ELIMINATION_DISTRIBUTION_H
#define DEF_ELIMINATION_DISTRIBUTION_H

#include <cstdint>
#include <vector>
#include <random>
#include <limits>


template <class T>
class elimination_distribution
{
    using element_type = T;
    using nb_of_site_type = int16_t;
    using dimension_type = size_t;

    std::vector<std::pair<element_type, nb_of_site_type>> site_bag;
public:
    elimination_distribution() = delete;
    elimination_distribution(std::vector<std::pair<element_type, nb_of_site_type>> site_bag) :
        site_bag{ site_bag.begin(), site_bag.end() } {}


    bool empty()
    {
        return site_bag.empty();
    }

    //ALWAYS check empty before using or else it will return the default constructor of element_type
    int operator()(std::default_random_engine& engine)
    {
        if (site_bag.empty())
        {
            return element_type{};
        }

        std::uniform_int_distribution<> sites_id_distribution{ 0, static_cast<int>(site_bag.size() - 1) };
        size_t chosen_site = sites_id_distribution(engine);
        auto site = site_bag[chosen_site].first;
        site_bag[chosen_site].second--;
        if (site_bag[chosen_site].second == 0)
        {
            site_bag.erase(site_bag.begin() + chosen_site);
        }

        return site;
    }
};
#endif