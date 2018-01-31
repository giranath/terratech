cmake_minimum_required(VERSION 3.9)

project(MapGeneration VERSION 0.1.0 DESCRIPTION "A map generator for games")

set(CMAKE_CXX_STANDARD 17)
set(ROOT ${CMAKE_SOURCE_DIR})

#==========================================================================
# OPTIONS
#==========================================================================
option(BUILD_DOC "Generate documentation")
option(BUILD_TEST "Build tests")

#==========================================================================
# MAIN LIBRARY
#==========================================================================
configure_file("${ROOT}/include/mapgen_config.h.in" 
               "${ROOT}/include/mapgen_config.h")
add_library(mapgen
	include/mapgen_config.h
	include/mapgen.h
	src/mapgen_api.cpp
	include/map.hpp
	src/map.cpp
	include/region.hpp
	src/region.cpp
	src/region.cpp
	include/perlin_noise.hpp
	src/perlin_noise.cpp
	include/biome_table.hpp
	src/biome_table.cpp)
set_target_properties(mapgen PROPERTIES VERSION ${PROJECT_VERSION})

if (BUILD_TEST)
	add_subdirectory(test)
endif ()

#==========================================================================
# DOCUMENTATION 
#==========================================================================
if (BUILD_DOC)
	# To generate the documentation, Doxygen is required
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		configure_file("${ROOT}/doc/Doxyfile.in"
		               "${ROOT}/doc/Doxyfile")
		
		add_custom_target(doc
		                  COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_SOURCE_DIR}/doc/Doxyfile"
		                  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		                  COMMENT "Generating API documentation with Doxygen"
		                  VERBATIM)

	endif(DOXYGEN_FOUND)
endif(BUILD_DOC)
