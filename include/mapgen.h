#ifndef DEF_MAPGEN_MAPGEN_H
#define DEF_MAPGEN_MAPGEN_H

#ifndef MAPGEN_NO_INCLUDES
#include "mapgen_config.h"
#include "mapgen_errors.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \file mapgen.h
 * This file contains MapGen's public C API
 */

/**================================================================
 * API VERSIONING
 * ================================================================
 * \page versioning API Versioning
 * MapGen uses Semantic Versioning 2.0.0 to indicate it's version.
 * Read https://semver.org/ for more informations.
 */

/** 
 * \brief Fetch a part of the current version
 * \return The major version
 * \ref versioning
 */
int mapgen_version_get_major();

/** 
 * \brief Fetch a part of the current version
 * \return The minor version
 * \ref versioning
 */
int mapgen_version_get_minor();

/** 
 * \brief Fetch a part of the current version
 * \return The patch version
 * \ref versioning
 */
int mapgen_version_get_patch();

/**================================================================
 * MAP
 * ================================================================
 * \page Map
 * The map is the basic structure of MapGen
 */
struct mapgen_map;
struct mapgen_layer;
struct mapgen_biome_table;

typedef void* mapgen_userdata;
typedef int mapgen_error;
typedef int mapgen_property_name;
typedef int mapgen_noise_type;
typedef struct mapgen_layer* mapgen_layer_handle;

#define MAPGEN_LAYER_INVALID NULL

// Noise types
enum {
    MAPGEN_NOISE_PERLIN
};

enum {
    MAPGEN_NOISE_SEED
};

/**
 * Instantiate a new map
 * @return The instantiated map or NULL if an error happened
 */
struct mapgen_map* mapgen_map_create();

/**
 * Attaches custom data to a map
 * @param map The map to attach some data to
 * @param userdata A pointer to some data
 */
void mapgen_map_attach_userdata(struct mapgen_map* map, mapgen_userdata userdata);

/**
 * Recuperates user data attached to a map
 * @param map The map on which is attached the user data
 * @return The attached user data
 */
mapgen_userdata mapgen_map_userdata(struct mapgen_map* map);

/**
 * Destroys a map
 * @param map The map to destroy
 */
void mapgen_map_destroy(struct mapgen_map* map);

mapgen_layer_handle mapgen_map_add_noise_layer(struct mapgen_map* map, mapgen_noise_type noise_type);
struct mapgen_biome_table* mapgen_map_enable_biomes(struct mapgen_map* map, mapgen_layer_handle axis_x, mapgen_layer_handle axis_y);
struct mapgen_biome_table* mapgen_map_biome_table(struct mapgen_map* map);

/**================================================================
 * Map layer
 * ================================================================
 * \page Error Handling
 * Describe how error handling is done
 */
mapgen_error mapgen_layer_set_property(mapgen_layer_handle layer, mapgen_property_name property, int value);


/**================================================================
 * Error handling
 * ================================================================
 * \page Error Handling
 * Describe how error handling is done
 */
const char* mapgen_last_error_msg(struct mapgen_map* map);
mapgen_error mapgen_last_error(struct mapgen_map* map);

#ifdef __cplusplus
}
#endif

#endif
