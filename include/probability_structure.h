#ifndef DEF_PROBABILITY_STRUCTURE_H
#define DEF_PROBABILITY_STRUCTURE_H
#include "map_type.h"

#include <map>
#include <vector>

template<class T>

class probability_structure
{
    std::map<biome_type, std::vector<std::pair<site_type, T>>> prob_strcuture{};
public:
    probability_structure() = default;

    //maybe create a varidic method
    void set_biome_and_site_occurence(const biome_type& biome_id, const std::vector<std::pair<site_type, T>>& sites_occurence)
    {
        prob_strcuture[biome_id] = sites_occurence;
    }

    void add_site_occurence(const biome_type& biome_id, const site_type& site_id, const T& occurence)
    {
        prob_strcuture[biome_id].push_back({ site_id, occurence });
    }

    void remove_biome(const biome_type& biome_id)
    {
        prob_strcuture.erase(biome_id);
    }

    std::vector<std::pair<site_type, T>> get_site_occurence(const biome_type& biome_id)
    {
        return prob_strcuture[biome_id];
    }

    size_t size() const
    {
        return prob_strcuture.size();
    }

    std::vector<biome_type> get_all_biomes()  
    {
        std::vector<biome_type> biomes;
        for (auto& v : prob_strcuture)
            biomes.push_back(v.first);
        return biomes;
    }
    
};
#endif