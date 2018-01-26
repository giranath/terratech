#ifndef DEF_REGION_H
#define DEF_REGION_H

#include <vector>
#include <cstdint>

class Region
{
    using id_type = uint16_t;
    std::vector<id_type> sites;
public:
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
