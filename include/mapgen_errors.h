#ifndef DEF_MAPGEN_MAPGEN_ERRORS_HPP
#define DEF_MAPGEN_MAPGEN_ERRORS_HPP

#ifdef __cplusplus
extern "C" {
#endif

enum {
    MAPGEN_ERROR_FAILED = -1,
    MAPGEN_ERROR_OK = 0,
    MAPGEN_ERROR_INVALID_VALUE,
    MAPGEN_ERROR_UNSUPPORTED_OPERATION,
    MAPGEN_ERROR_COUNT
};

#ifdef __cplusplus
}
#endif

#endif
