#ifndef DEF_MAPGEN_REGION_HPP
#define DEF_MAPGEN_REGION_HPP

#include <vector>
#include <cstdint>
#include <algorithm>

namespace mapgen {

class region {
    using id_type = uint16_t;
    std::vector<id_type> sites;
public:
    void add_site(id_type site_id);

    bool has_site(id_type site_id) const;
};

}

#endif
