#include "../include/region.hpp"

namespace mapgen {

void region::add_site(id_type site_id) {
    sites.push_back(site_id);
}

bool region::has_site(id_type site_id) const {
    return std::find(sites.begin(), sites.end(), site_id) != sites.end();
}

}