#define MAPGEN_NO_INCLUDES
#include <mapgen.h>
#include <tuple>
#include <iostream>

std::tuple<int, int, int> mapgen_version() {
    return std::make_tuple(mapgen_version_get_major(), mapgen_version_get_minor(), mapgen_version_get_patch());
};

struct MyUserData {
    int yeah;

    MyUserData(int y) : yeah(y) {}
};

int main() {
    auto version = mapgen_version();

    mapgen_map* map = mapgen_map_create();
    if(!map) {
        std::cerr << mapgen_last_error_msg(nullptr) << std::endl;
        return 1;
    }

    MyUserData userdata(312);
    mapgen_map_attach_userdata(map, &userdata);
    const int SEED = 13513;

    mapgen_layer_handle elevation = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);
    if(elevation != MAPGEN_LAYER_INVALID) {
        mapgen_layer_set_property(elevation, MAPGEN_NOISE_SEED, SEED);
    }
    else {
        std::cerr << "cannot create elevation layer: " << mapgen_last_error_msg(map) << std::endl;
        return 1;
    }

    mapgen_layer_handle humidity = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);
    if(humidity != MAPGEN_LAYER_INVALID) {
        mapgen_layer_set_property(humidity, MAPGEN_NOISE_SEED, SEED + 1);
    }
    else {
        std::cerr << "cannot create humidity layer: " << mapgen_last_error_msg(map) << std::endl;
        return 1;
    }

    mapgen_biome_table* biome_table = mapgen_map_enable_biomes(map, elevation, humidity);
    if(!biome_table) {
        std::cerr << "cannot enable biomes generation: " << mapgen_last_error_msg(map) << std::endl;
        return 1;
    }

    mapgen_map_destroy(map);
}