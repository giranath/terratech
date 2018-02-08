get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/mapgen-targets.cmake)
get_filename_component(mapgen_INCLUDE_DIRS "${SELF_DIR}/../../include" ABSOLUTE)

find_library(mapgen_LIBRARY
             mapgen
             PATHS "${SELF_DIR}/../../lib/"
             PATH_SUFFIXES mapgen
             NO_DEFAULT_PATH)

set(mapgen_LIBRARIES ${mapgen_LIBRARY})