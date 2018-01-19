#ifndef DEF_MAPGEN_MAPGEN_H
#define DEF_MAPGEN_MAPGEN_H

#include "mapgen_config.h"

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

#ifdef __cplusplus
}
#endif

#endif
