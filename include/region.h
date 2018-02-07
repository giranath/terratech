#ifndef DEF_REGION_H
#define DEF_REGION_H

#include "map_type.h"

#include <vector>
#include <cstdint>
#include <algorithm>

class region
{
    std::vector<site_type> sites;
    biome_type biome;
public:
    region() : biome{} {}
    region(biome_type biome) : biome{ biome } {}

    void set_biome(const biome_type& _biome)
    {
        biome = _biome;
    }
    biome_type get_biome() const
    {
        return biome;
    }

    void add_site(site_type site_id)
    {
        sites.push_back(site_id);
    }

    bool has_site(site_type site_id) const
    {
        return std::find(sites.begin(), sites.end(), site_id) != sites.end();
    }
    bool has_a_site() const
    {
        return !sites.empty();
    }
};

#endif
