#include "mapgen.h"
#include "map_type.hpp"
#include "map.hpp"
#include "perlin_noise.hpp"
#include "ppm.h"
#include "probability_structure.hpp"

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <type_traits>

#ifndef _WIN32

#include <cstring>
#include <stdlib.h>
#include <unistd.h>

#endif

static const char *VERSION = "2.1.0";

// Enumerates every supported biomes
enum {
    BIOME_RAIN_FOREST,
    BIOME_SWAMP,
    BIOME_SEASONAL_FOREST,
    BIOME_FOREST,
    BIOME_TAIGA,
    BIOME_WOODS,
    BIOME_SAVANNA,
    BIOME_DESERT,
    BIOME_GRASS_DESERT,
    BIOME_TUNDRA,
    BIOME_WATER,
    BIOME_DEEP_WATER,
    BIOME_COUNT
};

enum {
    SITE_NOTHING,
    SITE_HORSES,
    SITE_IRON,
    SITE_COAL,
    SITE_OIL,
    SITE_ALUMINUM,
    SITE_URANIUM,
    SITE_BANANA,
    SITE_CATTLE,
    SITE_DEER,
    SITE_SHEEP,
    SITE_WHEAT,
    SITE_STONE,
    SITE_PEARLS,
    SITE_FISH,

};
std::map<int, rgb> create_biome_color_mapping() {
    // Here we asign a color to a biome
    // see https://i.stack.imgur.com/vlvQQ.png for color code
    std::map<int, rgb> biome_color;
    biome_color[BIOME_RAIN_FOREST] = rgb(0, 0, 255);
    biome_color[BIOME_SWAMP] = rgb(63, 64, 255);
    biome_color[BIOME_SEASONAL_FOREST] = rgb(170, 0, 255);
    biome_color[BIOME_FOREST] = rgb(191, 64, 255);
    biome_color[BIOME_TAIGA] = rgb(255, 128, 255);
    biome_color[BIOME_WOODS] = rgb(255, 64, 191);
    biome_color[BIOME_SAVANNA] = rgb(255, 0, 170);
    biome_color[BIOME_DESERT] = rgb(255, 0, 0);
    biome_color[BIOME_GRASS_DESERT] = rgb(255, 97, 97);
    biome_color[BIOME_TUNDRA] = rgb(255, 191, 212);
    biome_color[BIOME_WATER] = rgb(0, 255, 230);
    biome_color[BIOME_DEEP_WATER] = rgb(0, 0, 255);


    return biome_color;
}

std::map<int, rgb> create_ressources_color_mapping() {
    // Here we asign a color to a biome
    // see https://i.stack.imgur.com/vlvQQ.png for color code
    std::map<int, rgb> ressource_color;
    ressource_color[SITE_HORSES] = rgb(102, 51, 0);
    ressource_color[SITE_IRON] = rgb(121, 121, 210);
    ressource_color[SITE_COAL] = rgb(89, 89, 89);
    ressource_color[SITE_OIL] = rgb(0, 0, 0);
    ressource_color[SITE_ALUMINUM] = rgb(244, 179, 66);
    ressource_color[SITE_URANIUM] = rgb(51, 204, 51);
    ressource_color[SITE_BANANA] = rgb(255, 255, 51);
    ressource_color[SITE_CATTLE] = rgb(8, 25, 38);
    ressource_color[SITE_DEER] = rgb(153, 51, 0);
    ressource_color[SITE_SHEEP] = rgb(255, 255, 255);
    ressource_color[SITE_WHEAT] = rgb(200, 200, 2);
    ressource_color[SITE_STONE] = rgb(194, 194, 163);
    ressource_color[SITE_PEARLS] = rgb(255, 255, 255);
    ressource_color[SITE_FISH] = rgb(247, 116, 122);

    return ressource_color;
}
int main(int argc, char *argv[]) {

    uint32_t seed{};
    mapgen::row_type width{};
    mapgen::column_type height{};

    std::cout << "Enter seed : ";
    std::cin >> seed;
    std::cout << std::endl;

    std::cout << "Enter width : ";
    std::cin >> width;
    std::cout << std::endl;

    std::cout << "Enter heigth : ";
    std::cin >> height;
    std::cout << std::endl;
    mapgen::biome_table biome_tab(6,4);
    mapgen::biome_axis altitude_tab(20);


    biome_tab.set_biome_at(0, 0, BIOME_TUNDRA);
    biome_tab.set_biome_at(1, 0, BIOME_TUNDRA);
    biome_tab.set_biome_at(2, 0, BIOME_GRASS_DESERT);
    biome_tab.set_biome_at(3, 0, BIOME_GRASS_DESERT);
    biome_tab.set_biome_at(4, 0, BIOME_DESERT);
    biome_tab.set_biome_at(5, 0, BIOME_DESERT);

    biome_tab.set_biome_at(0, 1, BIOME_TUNDRA);
    biome_tab.set_biome_at(1, 1, BIOME_TAIGA);
    biome_tab.set_biome_at(2, 1, BIOME_WOODS);
    biome_tab.set_biome_at(3, 1, BIOME_WOODS);
    biome_tab.set_biome_at(4, 1, BIOME_SAVANNA);
    biome_tab.set_biome_at(5, 1, BIOME_SAVANNA);

    biome_tab.set_biome_at(0, 2, BIOME_TAIGA);
    biome_tab.set_biome_at(1, 2, BIOME_TAIGA);
    biome_tab.set_biome_at(2, 2, BIOME_FOREST);
    biome_tab.set_biome_at(3, 2, BIOME_FOREST);
    biome_tab.set_biome_at(4, 2, BIOME_SEASONAL_FOREST);
    biome_tab.set_biome_at(5, 2, BIOME_SEASONAL_FOREST);

    biome_tab.set_biome_at(0, 3, BIOME_FOREST);
    biome_tab.set_biome_at(1, 3, BIOME_FOREST);
    biome_tab.set_biome_at(2, 3, BIOME_FOREST);
    biome_tab.set_biome_at(3, 3, BIOME_SWAMP);
    biome_tab.set_biome_at(4, 3, BIOME_RAIN_FOREST);
    biome_tab.set_biome_at(5, 3, BIOME_RAIN_FOREST);
    
    altitude_tab.set_biome_at(0, BIOME_DEEP_WATER);
    altitude_tab.set_biome_at(1, BIOME_DEEP_WATER);
    altitude_tab.set_biome_at(2, BIOME_WATER);
    altitude_tab.set_biome_at(3, BIOME_WATER);
    altitude_tab.set_biome_at(4, BIOME_WATER);
    altitude_tab.set_biome_at(5, BIOME_WATER);
    altitude_tab.set_biome_at(6, BIOME_WATER);
    altitude_tab.set_biome_at(12, BIOME_WATER);

    mapgen::map civ_map(width, height, seed, biome_tab, altitude_tab);
    civ_map.set_biome_with_table();

    mapgen::probability_structure<double> prob;
    prob.set_biome_and_site_occurence(BIOME_TUNDRA,       {{SITE_NOTHING, 200.0}, {SITE_DEER, 2}, {SITE_COAL,1}, {SITE_OIL,1} });
    prob.set_biome_and_site_occurence(BIOME_GRASS_DESERT, {{ SITE_NOTHING, 200.0 },{ SITE_HORSES, 1 },{ SITE_STONE, 1 } });
    prob.set_biome_and_site_occurence(BIOME_DESERT,       {{ SITE_NOTHING, 200.0 },{ SITE_URANIUM, 0.5 },{ SITE_IRON,1},{ SITE_OIL,3 } });
    prob.set_biome_and_site_occurence(BIOME_FOREST,       {{ SITE_NOTHING, 200.0 }, { SITE_DEER, 1 },{ SITE_CATTLE, 1 },{ SITE_IRON,1 }});
    prob.set_biome_and_site_occurence(BIOME_WOODS, { { SITE_NOTHING, 200.0 },{ SITE_DEER, 1 },{ SITE_CATTLE, 1 },{ SITE_IRON,1 } });
    prob.set_biome_and_site_occurence(BIOME_RAIN_FOREST,  {{ SITE_NOTHING, 200.0 }, { SITE_DEER, 1 }, { SITE_BANANA, 1.5 }, { SITE_URANIUM,0.5 }});
    prob.set_biome_and_site_occurence(BIOME_SAVANNA,      {{ SITE_NOTHING, 200.0 },{ SITE_CATTLE, 1 },{ SITE_WHEAT, 1.5 },{ SITE_STONE,0.5 } });
    prob.set_biome_and_site_occurence(BIOME_RAIN_FOREST,  {{ SITE_NOTHING, 200.0 },{ SITE_DEER, 1 },{ SITE_BANANA, 1.5 },{ SITE_SHEEP,0.5 }});
    prob.set_biome_and_site_occurence(BIOME_SEASONAL_FOREST, { { SITE_NOTHING, 200.0 },{ SITE_DEER, 1 },{ SITE_BANANA, 1.5 },{ SITE_SHEEP,0.5 } });
    prob.set_biome_and_site_occurence(BIOME_SWAMP, { { SITE_NOTHING, 200.0 },{ SITE_BANANA, 1 },{ SITE_FISH, 0.5 },{ SITE_COAL,0.5 } });
    prob.set_biome_and_site_occurence(BIOME_TAIGA, { { SITE_NOTHING, 200.0 },{ SITE_DEER, 2 },{ SITE_SHEEP, 1 },{ SITE_WHEAT,1 } });
    prob.set_biome_and_site_occurence(BIOME_WATER, { { SITE_NOTHING, 200.0 },{ SITE_FISH,1 } });
    prob.set_biome_and_site_occurence(BIOME_DEEP_WATER, { { SITE_NOTHING, 200.0 },{ SITE_FISH, 1 },{ SITE_PEARLS,1 } });

    civ_map.generate_by_weight_and_biome(prob);
    std::vector<std::vector<rgb>> image_vector(width, std::vector<rgb>(height));
    std::map<int, rgb> biome_map = create_biome_color_mapping();
    std::map<int, rgb> site_map = create_ressources_color_mapping();

    for (mapgen::row_type x = 0; x < width; ++x)
    {
        for (mapgen::column_type y = 0; y < width; ++y)
        {
            mapgen::biome_type biome = civ_map.get_biome_at(x, y);
            mapgen::site_type site = civ_map.get_sites_at(x,y).front();
            if (site == SITE_NOTHING)
            {
                image_vector[x][y] = biome_map[biome];
            }
            else
            {
                image_vector[x][y] = site_map[site];
            }
        }
    }

    std::ofstream file("water.ppm");
    ppm image(image_vector);
    image.write(file);
}

