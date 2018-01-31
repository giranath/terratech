#include "../include/mapgen.h"
#include "../include/mapgen_errors.h"
#include "../include/biome_table.hpp"
#include "../include/perlin_noise.hpp"

#include <vector>
#include <memory>

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
    mapgen::perlin_noise* noise;

    mapgen_layer(mapgen_map* owner, mapgen::perlin_noise* noise) noexcept
    : owner(owner)
    , noise(noise) {

    }
};

struct mapgen_biome_table {
    enum class type_t {
        Unknown,
        Table,
        Axis
    };

    mapgen_map* owner;
    mapgen_layer* x_axis;
    mapgen_layer* y_axis;
    type_t type;
    std::unique_ptr<mapgen::biome_table> table;
    std::unique_ptr<mapgen::biome_axis> axis;

    explicit mapgen_biome_table(mapgen_map* owner) noexcept
    : owner(owner)
    , x_axis{}
    , y_axis{}
    , type{type_t::Unknown}
    , table{}
    , axis{} {
    }
};

struct mapgen_map {
    std::vector<mapgen::perlin_noise> noises;
    std::vector<std::unique_ptr<mapgen_layer>> layers;
    int last_error;
    const char* last_error_msg;
    mapgen_biome_table biomes;
    mapgen_biome_table altitude_biomes;
    mapgen_userdata userdata;

    mapgen_map() noexcept
    : noises()
    , last_error(MAPGEN_ERROR_OK)
    , last_error_msg("")
    , biomes(this)
    , altitude_biomes(this)
    , userdata(nullptr) {
        noises.reserve(100);
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

int mapgen_map_biome_at(struct mapgen_map* map, float x, float y) {
    if(map) {
        if(map->altitude_biomes.axis) {
            // 1. Check altitude biome
            double altitude = (map->altitude_biomes.x_axis->noise->octave_noise(x, y, 0.f, 8, 0.55f) + 0.707) / 1.414;

            int altitude_biome = map->altitude_biomes.axis->biome_with(altitude);

            if(altitude_biome == mapgen::biome_axis::NO_SPECIAL_BIOME) {
                if(map->biomes.table) {
                    double col = (map->biomes.x_axis->noise->octave_noise(x, y, 0.f, 8, 0.55f) + 0.707) / 1.414;
                    double row = (map->biomes.y_axis->noise->octave_noise(x, y, 0.f, 8, 0.55f) + 0.707) / 1.414;

                    return map->biomes.table->biome_with(col, row);
                }
                else {
                    map->last_error = MAPGEN_ERROR_INVALID_VALUE;
                    map->last_error_msg = "biomes are disabled";
                }
            }
            else {
                return altitude_biome;
            }
        }
        else {
            map->last_error = MAPGEN_ERROR_INVALID_VALUE;
            map->last_error_msg = "biomes are disabled";
        }
    }

    return MAPGEN_ERROR_FAILED;
}

mapgen_layer_handle mapgen_map_add_noise_layer(struct mapgen_map* map, mapgen_noise_type noise_type) {
    if(map) {
        mapgen_layer_handle layer_handle = nullptr;

        switch(noise_type) {
            case MAPGEN_NOISE_PERLIN:
                map->noises.emplace_back();
                map->layers.push_back(std::make_unique<mapgen_layer>(map, &map->noises.back()));
                layer_handle = map->layers.back().get();
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

struct mapgen_biome_table* mapgen_map_enable_biomes(struct mapgen_map* map, int x_step, int y_step, mapgen_layer_handle axis_x, mapgen_layer_handle axis_y) {
    if(map) {
        mapgen_biome_table* table = &map->biomes;
        table->type = mapgen_biome_table::type_t::Table;
        table->table = std::make_unique<mapgen::biome_table>(x_step, y_step);
        table->x_axis = axis_x;
        table->y_axis = axis_y;

        return table;
    }

    return nullptr;
}

struct mapgen_biome_table* mapgen_map_enable_altitude(struct mapgen_map* map, int step, mapgen_layer_handle altitude_layer) {
    if(map) {
        mapgen_biome_table* table = &map->altitude_biomes;
        table->type = mapgen_biome_table::type_t::Axis;
        table->axis = std::make_unique<mapgen::biome_axis>(step);
        table->x_axis = altitude_layer;

        return table;
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

//================================================================
// Biomes table
//================================================================
mapgen_error mapgen_biome_table_set(struct mapgen_biome_table* table, int x, int y, int biome) {
    if(table) {
        if(table->type == mapgen_biome_table::type_t::Table && table->table) {
            table->table->set_biome_at(x, y, biome);
            return MAPGEN_ERROR_OK;
        }
        else if(table->type == mapgen_biome_table::type_t::Axis && table->axis) {
            table->axis->set_biome_at(x, biome);
            return MAPGEN_ERROR_OK;
        }
        else {
            table->owner->last_error_msg = "biome table not enabled";
            table->owner->last_error = MAPGEN_ERROR_INVALID_VALUE;
        }
    }

    return MAPGEN_ERROR_FAILED;
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
