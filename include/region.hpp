#ifndef DEF_MAPGEN_REGION_HPP
#define DEF_MAPGEN_REGION_HPP

#include <vector>
#include <cstdint>
#include <algorithm>

namespace mapgen {

class region {
    using biome_type = int;
    using id_type = uint16_t;
    
    biome_type biome;
    std::vector<id_type> sites;
    
public:
    region() :biome{} {}
    region(biome_type biome) : biome{ biome }{}
    region(biome_type biome, std::vector<id_type> sites) : biome{ biome }, sites{sites.being(), sites.end()} {}

    void add_site(id_type site_id);

    bool has_site(id_type site_id) const;
};

}

#endif
