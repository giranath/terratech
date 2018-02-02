#ifndef DEF_REGION_H
#define DEF_REGION_H

#include <vector>
#include <cstdint>
#include <algorithm>

class region
{
    using id_type = uint16_t;
    using biome_type = uint16_t;
    std::vector<id_type> sites;
    biome_type biome;
public:
    region() : biome{} {}
    region(biome_type biome) : biome{ biome } {}

    void set_biome(const biome_type& _biome)
    {
        biome = _biome;
    }
    biome_type get_biome()
    {
        return biome;
    }

    void add_site(id_type site_id)
    {
        sites.push_back(site_id);
    }

    bool has_site(id_type site_id)
    {
        return std::find(sites.begin(), sites.end(), site_id) != sites.end();
    }
};

#endif
