#include "../include/mapgen.h"
#include "../include/mapgen_errors.h"
#include "../include/biome_table.hpp"
#include "../include/perlin_noise.h"

#include <vector>

extern "C" {

//==============================================================================
// VERSIONING
//==============================================================================

int mapgen_version_get_major() {
    return MAPGEN_VERSION_MAJOR;
}

int mapgen_version_get_minor() {
    return MAPGEN_VERSION_MINOR;
}

int mapgen_version_get_patch() {
    return MAPGEN_VERSION_PATCH;
}

//==============================================================================
// MAP
//==============================================================================
struct mapgen_layer {
    mapgen_map* owner;
    Perlin_noise* noise;

    mapgen_layer(mapgen_map* owner, Perlin_noise* noise)
    : owner(owner)
    , noise(noise) {

    }
};

struct mapgen_biome_table {
};

struct mapgen_map {
    std::vector<Perlin_noise> noises;
    std::vector<mapgen_layer> layers;
    int last_error;
    const char* last_error_msg;
    mapgen_userdata userdata;

    mapgen_map()
    : noises()
    , last_error(MAPGEN_ERROR_OK)
    , last_error_msg("")
    , userdata(nullptr) {

    }
};

struct mapgen_map* mapgen_map_create() {
    auto* map = new mapgen_map();

    return map;
}

void mapgen_map_destroy(struct mapgen_map* map) {
    delete map;
}

void mapgen_map_attach_userdata(struct mapgen_map* map, mapgen_userdata userdata) {
    if(map) {
        map->userdata = userdata;
    }
}

mapgen_userdata mapgen_map_userdata(struct mapgen_map* map) {
    mapgen_userdata data = nullptr;

    if(map) {
        data = map->userdata;
    }

    return data;
}

mapgen_layer_handle mapgen_map_add_noise_layer(struct mapgen_map* map, mapgen_noise_type noise_type) {
    if(map) {
        mapgen_layer_handle layer_handle = nullptr;

        switch(noise_type) {
            case MAPGEN_NOISE_PERLIN:
                map->noises.emplace_back();
                map->layers.emplace_back(map, &map->noises.back());
                layer_handle = &map->layers.back();
                break;
            default:
                map->last_error = MAPGEN_ERROR_INVALID_VALUE;
                map->last_error_msg = "unsupported noise type";
                break;
        }

        return layer_handle;
    }

    return nullptr;
}

struct mapgen_biome_table* mapgen_map_enable_biomes(struct mapgen_map* map, mapgen_layer_handle axis_x, mapgen_layer_handle axis_y) {
    if(map) {
        map->last_error = MAPGEN_ERROR_UNSUPPORTED_OPERATION;
        map->last_error_msg = "not implemented";
    }
    return nullptr;
}

struct mapgen_biome_table* mapgen_map_biome_table(struct mapgen_map* map) {
    if(map) {
        map->last_error = MAPGEN_ERROR_UNSUPPORTED_OPERATION;
        map->last_error_msg = "not implemented";
    }
    return nullptr;
}

//================================================================
// Map layer
//================================================================
mapgen_error mapgen_layer_set_property(mapgen_layer_handle layer, mapgen_property_name property, int value) {
    mapgen_error err = MAPGEN_ERROR_FAILED;

    if(layer) {
        switch(property) {
            case MAPGEN_NOISE_SEED:
                layer->noise->seed(static_cast<uint32_t>(value));
                err = MAPGEN_ERROR_OK;
                break;
            default:
                layer->owner->last_error = MAPGEN_ERROR_INVALID_VALUE;
                layer->owner->last_error_msg = "unrecognized property";
                break;
        }

    }

    return err;
}

//==============================================================================
// ERROR HANDLING
//==============================================================================
const char* mapgen_last_error_msg(struct mapgen_map* map) {
    if(map) {
        return map->last_error_msg;
    }
    return nullptr;
}

int mapgen_last_error(struct mapgen_map* map) {
    if(map) {
        return map->last_error;
    }

    return MAPGEN_ERROR_INVALID_VALUE;
}
}
