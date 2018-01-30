#include "../include/mapgen.h"

extern "C" {

int mapgen_version_get_major() {
    return MAPGEN_VERSION_MAJOR;
}

int mapgen_version_get_minor() {
    return MAPGEN_VERSION_MINOR;
}

int mapgen_version_get_patch() {
    return MAPGEN_VERSION_PATCH;
}

}
