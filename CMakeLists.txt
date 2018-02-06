cmake_minimum_required(VERSION 3.9)

project(MapGeneration VERSION 0.2.0 DESCRIPTION "A map generator for games")

set(CMAKE_CXX_STANDARD 14)

#==========================================================================
# OPTIONS
#==========================================================================
option(BUILD_DOC "Generate documentation")
option(BUILD_TEST "Build tests")
option(BUILD_PACKAGE "Generate distribution packages")

configure_file("${CMAKE_SOURCE_DIR}/mapgen-config-version.cmake.in"
               "${CMAKE_SOURCE_DIR}/mapgen-config-version.cmake"
               @ONLY)

#==========================================================================
# MAIN LIBRARY
#==========================================================================
configure_file("${CMAKE_SOURCE_DIR}/include/mapgen_config.h.in"
               "${CMAKE_SOURCE_DIR}/include/mapgen_config.h")
add_library(mapgen
	include/mapgen_errors.h
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
set_target_properties(mapgen PROPERTIES VERSION ${PROJECT_VERSION}
                                        SOVERSION ${PROJECT_VERSION_MAJOR})

# Install generated binaries
install(TARGETS mapgen
        EXPORT mapgen-targets
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib/mapgen
        ARCHIVE DESTINATION lib/mapgen)
install(DIRECTORY include/
        DESTINATION include/mapgen
        FILES_MATCHING PATTERN "*.h")
install(EXPORT mapgen-targets
        DESTINATION lib/mapgen)
install(FILES mapgen-config.cmake mapgen-config-version.cmake
        DESTINATION lib/mapgen)

if (BUILD_TEST)
	add_subdirectory(test)
endif ()

set(ADD_DOC_DEP false)
#==========================================================================
# DOCUMENTATION 
#==========================================================================
if (BUILD_DOC)
	# To generate the documentation, Doxygen is required
	find_package(Doxygen)
	if (DOXYGEN_FOUND)
		configure_file("${CMAKE_SOURCE_DIR}/doc/Doxyfile.in"
		               "${CMAKE_SOURCE_DIR}/doc/Doxyfile")
		
		add_custom_target(doc
		                  COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_SOURCE_DIR}/doc/Doxyfile"
		                  WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
		                  COMMENT "Generating API documentation with Doxygen"
		                  VERBATIM)
		install(DIRECTORY ${CMAKE_BINARY_DIR}/generated_doc/man/man3
		        DESTINATION share/man)
		install(DIRECTORY ${CMAKE_BINARY_DIR}/generated_doc/html
		        DESTINATION share)
		set(ADD_DOC_DEP true)
	endif(DOXYGEN_FOUND)
endif(BUILD_DOC)

#==========================================================================
# PACKAGING
#==========================================================================
if (BUILD_PACKAGE)
	include(CPack)
endif()