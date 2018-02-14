#include <mapgen.h>

#include <ppm.h>
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


mapgen_map* create_map(int seed);

void setup_biomes_table(mapgen_biome_table* table);

struct Arguments {
    std::size_t width = 768;
    std::size_t height = 768;
    uint32_t seed = 0;
    bool seed_from_stdin = false;

    Arguments() = default;
};

void show_help(const char *exec) {
    std::cout << "usage: " << exec << "[OPTION]..." << std::endl;
    std::cout << "generate a biome distribution texture to standard output.\n" << std::endl;

    std::cout << "options:" << std::endl;
    std::cout << "  -W <width>      Set the texture's width in pixels  [768]" << std::endl;
    std::cout << "  -H <height>     Set the texture's height in pixels [768]" << std::endl;
    std::cout << "  -s <seed>|-     Use the specified seed to generate the noise" << std::endl;
    std::cout << "                  if - is used, the seed will be read from stdin" << std::endl;
    std::cout << "  -v              Display the version" << std::endl;
    std::cout << "  -h              Display this help" << std::endl;
}

void show_version() {
    std::cout << "biometexgen (mapgen " MAPGEN_VERSION_STR ") " << VERSION << std::endl;
}

Arguments parse_arguments(int argc, char *const argv[]) {
    Arguments args;

#ifndef _WIN32
    int c;
    while ((c = getopt(argc, argv, "+hvW:H:s:")) != -1) {
        switch (c) {
            case 'h':
                show_help(argv[0]);
                exit(0);
                break;
            case 'v':
                show_version();
                exit(0);
                break;
            case 'W':
                args.width = std::atoi(optarg);
                break;
            case 'H':
                args.height = std::atoi(optarg);
                break;
            case 's':
                if (std::strcmp(optarg, "-") == 0) {
                    args.seed_from_stdin = true;
                } else {
                    args.seed = std::atoi(optarg);
                }
                break;
            default:
                show_help(argv[0]);
                exit(1);
                break;
        }
    }
#endif

    return args;
}

std::vector<std::vector<rgb>> create_texture_from_map(mapgen_map* map, std::size_t texture_width, std::size_t texture_height);

int main(int argc, char *argv[]) {
    auto args = parse_arguments(argc, argv);

    std::size_t texture_width = args.width;
    std::size_t texture_height = args.height;

    uint32_t seed = args.seed;
    if (args.seed_from_stdin) {
        std::cin >> seed;
    }

    mapgen_map* map = create_map(seed);
    if(!map) {
        throw std::runtime_error("cannot initialize map");
    }

    // TEXTURE GENERATION
    // Generate images
    auto pixels = create_texture_from_map(map, texture_width, texture_height);
    mapgen_map_destroy(map);

    // Dump image
    ppm image(pixels);
    std::cout << image << std::endl;
}

mapgen_map* create_map(int seed) {
    mapgen_map* map = mapgen_map_create();
    if(!map) {
        std::cerr << mapgen_last_error_msg(nullptr) << std::endl;
        return nullptr;
    }

    mapgen_layer_handle temperature = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);
    if(temperature != MAPGEN_LAYER_INVALID) {
        mapgen_layer_set_property(temperature, MAPGEN_NOISE_SEED, seed);
    }
    else {
        std::cerr << "cannot create elevation layer: " << mapgen_last_error_msg(map) << std::endl;
        return nullptr;
    }

    mapgen_layer_handle humidity = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);
    if(humidity != MAPGEN_LAYER_INVALID) {
        mapgen_layer_set_property(humidity, MAPGEN_NOISE_SEED, seed + 1);
    }
    else {
        std::cerr << "cannot create humidity layer: " << mapgen_last_error_msg(map) << std::endl;
        return nullptr;
    }

    mapgen_layer_handle altitude = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);
    if(altitude) {
        mapgen_layer_set_property(altitude, MAPGEN_NOISE_SEED, seed + 2);
    }
    else {
        std::cerr << "cannot create altitude layer: " << mapgen_last_error_msg(map) << std::endl;
        return nullptr;
    }

    mapgen_biome_table* biome_table = mapgen_map_enable_biomes(map, 6, 4, temperature, humidity);
    if(!biome_table) {
        std::cerr << "cannot enable biomes generation: " << mapgen_last_error_msg(map) << std::endl;
        return nullptr;
    }

    // SETUP BIOME TABLE
    setup_biomes_table(biome_table);

    mapgen_biome_table* altitude_table = mapgen_map_enable_altitude(map, 20, altitude);
    if(altitude_table) {
        mapgen_biome_table_set(altitude_table, 0,  0, BIOME_DEEP_WATER);
        mapgen_biome_table_set(altitude_table, 1,  0, BIOME_DEEP_WATER);
        mapgen_biome_table_set(altitude_table, 2,  0, BIOME_WATER);
        mapgen_biome_table_set(altitude_table, 3,  0, BIOME_WATER);
        mapgen_biome_table_set(altitude_table, 4,  0, BIOME_WATER);
        mapgen_biome_table_set(altitude_table, 5,  0, BIOME_WATER);
        mapgen_biome_table_set(altitude_table, 6,  0, BIOME_WATER);
        mapgen_biome_table_set(altitude_table, 12, 0, BIOME_WATER);
    }

    return map;
}

void setup_biomes_table(mapgen_biome_table* biome_table) {
    // Here we create a biome repartition table
    // 1.0 ^
    //     |  +---------+---------+---------+---------+---------+---------+
    //     H  |              FOREST         |  SWAMP  |    RAIN FOREST    |
    //     U  +---------+---------+         +---------+---------+---------+
    //     M  |                   |                   |  SEASONAL FOREST  |
    //     I  +---------+         +---------+---------+---------+---------+
    //     D  |         |  TAIGA  |       WOODS       |      SAVANNA      |
    //     I  +         +---------+---------+---------+---------+---------+
    //     T  |       TUNDRA      |    GRASS DESERT   |       DESERT      |
    //     Y  +---------+---------+---------+---------+---------+---------+
    //     |
    //     +----------------------- TEMPERATURE --------------------------->
    // 0.0                                                             1.0
    //
    mapgen_biome_table_set(biome_table, 0, 0, BIOME_TUNDRA);
    mapgen_biome_table_set(biome_table, 1, 0, BIOME_TUNDRA);
    mapgen_biome_table_set(biome_table, 2, 0, BIOME_GRASS_DESERT);
    mapgen_biome_table_set(biome_table, 3, 0, BIOME_GRASS_DESERT);
    mapgen_biome_table_set(biome_table, 4, 0, BIOME_DESERT);
    mapgen_biome_table_set(biome_table, 5, 0, BIOME_DESERT);

    mapgen_biome_table_set(biome_table, 0, 1, BIOME_TUNDRA);
    mapgen_biome_table_set(biome_table, 1, 1, BIOME_TAIGA);
    mapgen_biome_table_set(biome_table, 2, 1, BIOME_WOODS);
    mapgen_biome_table_set(biome_table, 3, 1, BIOME_WOODS);
    mapgen_biome_table_set(biome_table, 4, 1, BIOME_SAVANNA);
    mapgen_biome_table_set(biome_table, 5, 1, BIOME_SAVANNA);

    mapgen_biome_table_set(biome_table, 0, 2, BIOME_TAIGA);
    mapgen_biome_table_set(biome_table, 1, 2, BIOME_TAIGA);
    mapgen_biome_table_set(biome_table, 2, 2, BIOME_FOREST);
    mapgen_biome_table_set(biome_table, 3, 2, BIOME_FOREST);
    mapgen_biome_table_set(biome_table, 4, 2, BIOME_SEASONAL_FOREST);
    mapgen_biome_table_set(biome_table, 5, 2, BIOME_SEASONAL_FOREST);

    mapgen_biome_table_set(biome_table, 0, 3, BIOME_FOREST);
    mapgen_biome_table_set(biome_table, 1, 3, BIOME_FOREST);
    mapgen_biome_table_set(biome_table, 2, 3, BIOME_FOREST);
    mapgen_biome_table_set(biome_table, 3, 3, BIOME_SWAMP);
    mapgen_biome_table_set(biome_table, 4, 3, BIOME_RAIN_FOREST);
    mapgen_biome_table_set(biome_table, 5, 3, BIOME_RAIN_FOREST);
}

std::map<int, rgb> create_biome_color_mapping() {
    // Here we asign a color to a biome
    // see https://i.stack.imgur.com/vlvQQ.png for color code
    std::map<int, rgb> biome_color;
    biome_color[BIOME_RAIN_FOREST] =     rgb(0,   0,   255);
    biome_color[BIOME_SWAMP] =           rgb(63,  64,  255);
    biome_color[BIOME_SEASONAL_FOREST] = rgb(170, 0,   255);
    biome_color[BIOME_FOREST] =          rgb(191, 64,  255);
    biome_color[BIOME_TAIGA] =           rgb(255, 128, 255);
    biome_color[BIOME_WOODS] =           rgb(255, 64,  191);
    biome_color[BIOME_SAVANNA] =         rgb(255, 0,   170);
    biome_color[BIOME_DESERT] =          rgb(255, 0,   0);
    biome_color[BIOME_GRASS_DESERT] =    rgb(255, 97,  97);
    biome_color[BIOME_TUNDRA] =          rgb(255, 191, 212);
    biome_color[BIOME_WATER] =           rgb(0,   255, 230);
    biome_color[BIOME_DEEP_WATER] =      rgb(0,   0,   255);

    return biome_color;
}

std::vector<std::vector<rgb>> create_texture_from_map(mapgen_map* map, std::size_t texture_width, std::size_t texture_height) {
    std::map<int, rgb> biome_color = create_biome_color_mapping();

    std::vector<std::vector<rgb>> pixels(texture_width);
    for(std::size_t x = 0; x < texture_width; ++x) {
        pixels[x].reserve(texture_height);
        for(std::size_t y = 0; y < texture_height; ++y) {
            float pos_x = static_cast<float>(x) / texture_width;
            float pos_y = static_cast<float>(y) / texture_height;

            int biome = mapgen_map_biome_at(map, pos_x, pos_y);

            pixels[x].push_back(biome_color[biome]);
        }
    }

    return pixels;
}
