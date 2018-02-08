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

/*
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

/**
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

/**
 *
 */
enum {
    MAPGEN_NOISE_SEED
};

/**
 * \brief Instantiate a new map
 * \return The instantiated map or NULL if an error happened
 */
struct mapgen_map* mapgen_map_create();

/**
 * \brief Attaches custom data to a map
 * \param map The map to attach some data to
 * \param userdata A pointer to some data
 */
void mapgen_map_attach_userdata(struct mapgen_map* map, mapgen_userdata userdata);

/**
 * \brief Recuperates user data attached to a map
 * \param map The map on which is attached the user data
 * \return The attached user data
 */
mapgen_userdata mapgen_map_userdata(struct mapgen_map* map);

/**
 * \brief Calculates the biome at a specific world coordinate
 * \param map The map where to calculate the biome
 * \param x The x coordinate
 * \param y The y coordinate
 * \return The calculated biome at the specified location
 */
int mapgen_map_biome_at(struct mapgen_map* map, float x, float y);

/**
 * \brief Destroys a map
 * \param map The map to destroy
 */
void mapgen_map_destroy(struct mapgen_map* map);

/**
 * \brief Add a noise layer to the map
 * \param map The map where to add the layer
 * \param noise_type The type of noise to create
 * \return An handle to the map layer
 * \ref noise_types
 */
mapgen_layer_handle mapgen_map_add_noise_layer(struct mapgen_map* map, mapgen_noise_type noise_type);

/**
 * \brief Enables biome generation on a map
 * \param map The map on which biome generation is enabled
 * \param x_step Defines the number of cells for the x axis in the biomes table
 * \param y_step Defines the number of cells for the y axis in the biomes table
 * \param axis_x Specifies the layer to use as the x axis of the biomes table
 * \param axis_y Specifies the layer to use as the y axis of the biomes table
 * \return The generated biome table. Read more on \ref biomes_table
 */
struct mapgen_biome_table* mapgen_map_enable_biomes(struct mapgen_map* map, int x_step, int y_step, mapgen_layer_handle axis_x, mapgen_layer_handle axis_y);

/**
 * \brief Enables altitude based biome generation on a map
 * \param map The map on which altitude based biome generation is enabled
 * \param step Defines the number of steps for the axis
 * \param altitude_layer Specifies the layer to use a the axis
 * \return The generated biome table. Read more on \ref biomes_table
 */
struct mapgen_biome_table* mapgen_map_enable_altitude(struct mapgen_map* map, int step, mapgen_layer_handle altitude_layer);

/**
 * \page noise_types Noise types
 * Enumerates supported noise types
 */

/**
 * \page map_layer Map layer
 * Describe what are map layers
 */

/**
 * \brief Set the property of a map layer
 * @param layer The layer to modify
 * @param property The name of the property to change
 * @param value The new value of the property
 * @return MAPGEN_ERROR_OK on success. Otherwise MAPGEN_ERROR_FAILED. Read more on \ref error_handling
 */
mapgen_error mapgen_layer_set_property(mapgen_layer_handle layer, mapgen_property_name property, int value);

/**
 * \page biomes_table Biomes table
 * A biome table is a simple representation of biome distribution.\n
 * It works by dividing two axis into blocks creating a table.\n
 * For exemple, let's say you want to create this biome distribution table:
 * <table>
 *  <caption>Simple biome table</caption>
 *  <tr>
 *      <th colspan="2"></th>
 *      <th colspan="6">Temperature</th>
 *  </tr>
 *  <tr>
 *     <th rowspan="5">Humidity</th>
 *     <th>Percent</th>
 *     <th>0.1667</th>
 *     <th>0.3333</th>
 *     <th>0.5</th>
 *     <th>0.6667</th>
 *     <th>0.8334</th>
 *     <th>1.0</th>
 *  </tr>
 *  <tr>
 *    <th>1.0</th>
 *    <td colspan="3">FOREST</td>
 *    <td>SWAMP</td>
 *    <td colspan="2">RAIN FOREST</td>
 *  </tr>
 *  <tr>
 *    <th>0.75</th>
 *    <td colspan="2">TAIGA</td>
 *    <td colspan="2">FOREST</td>
 *    <td colspan="2">SEASONAL_FOREST</td>
 *  </tr>
 *  <tr>
 *    <th>0.5</th>
 *    <td>TUNDRA</td>
 *    <td>TAIGA</td>
 *    <td colspan="2">WOODS</td>
 *    <td colspan="2">SAVANNA</td>
 *  </tr>
 *  <tr>
 *    <th>0.25</th>
 *    <td colspan="2">TUNDRA</td>
 *    <td colspan="2">GRASS DESERT</td>
 *    <td colspan="2">DESERT</td>
 *  </tr>
 * </table>
 * You need to write the following code:
 * \code{.c}
// Create a map
mapgen_map* map = mapgen_map_create();

// Attach noises to the map
mapgen_layer_handle temperature = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);
mapgen_layer_handle humidity = mapgen_map_add_noise_layer(map, MAPGEN_NOISE_PERLIN);

// Create a biome table in the map
mapgen_biome_table* biome_table = mapgen_map_enable_biomes(map, 6, 4, temperature, humidity);

// Bottom row
mapgen_biome_table_set(biome_table, 0, 0, BIOME_TUNDRA);
mapgen_biome_table_set(biome_table, 1, 0, BIOME_TUNDRA);
mapgen_biome_table_set(biome_table, 2, 0, BIOME_GRASS_DESERT);
mapgen_biome_table_set(biome_table, 3, 0, BIOME_GRASS_DESERT);
mapgen_biome_table_set(biome_table, 4, 0, BIOME_DESERT);
mapgen_biome_table_set(biome_table, 5, 0, BIOME_DESERT);

// Second row
mapgen_biome_table_set(biome_table, 0, 1, BIOME_TUNDRA);
mapgen_biome_table_set(biome_table, 1, 1, BIOME_TAIGA);
mapgen_biome_table_set(biome_table, 2, 1, BIOME_WOODS);
mapgen_biome_table_set(biome_table, 3, 1, BIOME_WOODS);
mapgen_biome_table_set(biome_table, 4, 1, BIOME_SAVANNA);
mapgen_biome_table_set(biome_table, 5, 1, BIOME_SAVANNA);

// Third row
mapgen_biome_table_set(biome_table, 0, 2, BIOME_TAIGA);
mapgen_biome_table_set(biome_table, 1, 2, BIOME_TAIGA);
mapgen_biome_table_set(biome_table, 2, 2, BIOME_FOREST);
mapgen_biome_table_set(biome_table, 3, 2, BIOME_FOREST);
mapgen_biome_table_set(biome_table, 4, 2, BIOME_SEASONAL_FOREST);
mapgen_biome_table_set(biome_table, 5, 2, BIOME_SEASONAL_FOREST);

// Top row
mapgen_biome_table_set(biome_table, 0, 3, BIOME_FOREST);
mapgen_biome_table_set(biome_table, 1, 3, BIOME_FOREST);
mapgen_biome_table_set(biome_table, 2, 3, BIOME_FOREST);
mapgen_biome_table_set(biome_table, 3, 3, BIOME_SWAMP);
mapgen_biome_table_set(biome_table, 4, 3, BIOME_RAIN_FOREST);
mapgen_biome_table_set(biome_table, 5, 3, BIOME_RAIN_FOREST);
 * \endcode
 */

/**
 * \brief Modifies the value of a cell in a biome table
 * @param table The table to modify
 * @param x The x coordinate of the cell to update
 * @param y The y coordinate of the cell to update
 * @param biome The biome value of the cell
 * @return MAPGEN_ERROR_OK on success. Otherwise MAPGEN_ERROR_FAILED. Read more on \ref error_handling
 */
mapgen_error mapgen_biome_table_set(struct mapgen_biome_table* table, int x, int y, int biome);

/**
 * \page error_handling Error Handling
 * In MapGen, each map is responsible to store it's error state when one occures.\n
 * An error is indicated by a code where the values are defined in mapgen_errors.h.\n
 * Functions that returns a mapgen_error will return MAPGEN_ERROR_OK when the operation succeeded
 * or MAPGEN_ERROR_FAILED if an error as occured.\n
 * A more precise error code can be retrieved with mapgen_last_error and a descriptive message can be
 * retrieved with mapgen_last_error_msg.
 * \see mapgen_last_error
 * \see mapgen_last_error_msg
 */

/**
 * \brief Returns the last error's descriptive message
 * \param map The map where the error happened
 * \return The last error's description encoded as a null terminated string
 */
const char* mapgen_last_error_msg(struct mapgen_map* map);

/**
 * \brief Returns the last error code
 * Read more about error codes in mapgen_errors.h
 * \param map The map where the error happened
 * \return The last error code
 * \ref mapgen_errors.h
 */
mapgen_error mapgen_last_error(struct mapgen_map* map);

#ifdef __cplusplus
}
#endif

#endif
