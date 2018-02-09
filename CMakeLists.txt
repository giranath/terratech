cmake_minimum_required(VERSION 3.9)

project(MapGeneration VERSION 0.2.0 DESCRIPTION "A map generator for games")

set(CMAKE_CXX_STANDARD 14)

# If library name is changed, do not forget to update:
# - mapgen-config.cmake
# - the namespace in C++ files
# - rename API

#==========================================================================
# OPTIONS
#==========================================================================
option(BUILD_DOC "Generate documentation")
option(BUILD_TEST "Build tests")
option(BUILD_PACKAGE "Generate distribution packages")
option(BUILD_WEBPAGE "Generate Web pages")

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
        COMPONENT libraries
        LIBRARY DESTINATION lib/mapgen
        COMPONENT libraries
        ARCHIVE DESTINATION lib/mapgen
        COMPONENT libraries)
install(DIRECTORY include/
        DESTINATION include/mapgen
        COMPONENT headers
        FILES_MATCHING PATTERN "*.h")
install(EXPORT mapgen-targets
        DESTINATION lib/mapgen
        COMPONENT cmake)
install(FILES mapgen-config.cmake mapgen-config-version.cmake
        DESTINATION lib/mapgen
        COMPONENT cmake)

if (BUILD_TEST)
	add_subdirectory(test)
endif ()

#==========================================================================
# DOCUMENTATION 
#==========================================================================
set(INSTALL_DOC_COMPONENT OFF)
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
		        DESTINATION share/man
		        COMPONENT documentation)
		install(DIRECTORY ${CMAKE_BINARY_DIR}/generated_doc/html
		        DESTINATION share
		        COMPONENT documentation)
		set(INSTALL_DOC_COMPONENT ON)
	endif(DOXYGEN_FOUND)
endif(BUILD_DOC)

if (BUILD_WEBPAGE)
	find_package(BASH)

	configure_file("${CMAKE_SOURCE_DIR}/doc/web/index.html.in" "${CMAKE_SOURCE_DIR}/doc/web/index.html")
	configure_file("${CMAKE_SOURCE_DIR}/doc/web/download.html.in" "${CMAKE_SOURCE_DIR}/doc/web/download.html")
	configure_file("${CMAKE_SOURCE_DIR}/doc/project_info.txt.in" "${CMAKE_SOURCE_DIR}/doc/project_info.txt")

	if(${BASH_FOUND})
		add_custom_target(deploy
		                  COMMAND ${BASH_EXECUTABLE} "script/deploy.sh"
		                  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
		                  COMMENT "Deploying library"
		                  VERBATIM)
	endif()
endif(BUILD_WEBPAGE)

#==========================================================================
# PACKAGING
#==========================================================================
if (BUILD_PACKAGE)
	include(InstallRequiredSystemLibraries)
	set(CPACK_PACKAGE_NAME "mapgen")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${CMAKE_PROJECT_DESCRIPTION}")
	set(CPACK_PACKAGE_VENDOR "Nathan Giraldeau & Dominic Nault")
	set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
	#set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/Copyright.txt")
	set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
	set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
	set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

	set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "Public headers")
	set(CPACK_COMPONENT_HEADERS_DESCRIPTION "Installs the librarie's public headers.")
	set(CPACK_COMPONENT_HEADERS_REQUIRED True)

	set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Development libraries")
	set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Installs the development libraries")
	set(CPACK_COMPONENT_LIBRARIES_REQUIRED True)

	set(CPACK_COMPONENT_CMAKE_DISPLAY_NAME "CMake related files")
	set(CPACK_COMPONENT_CMAKE_DESCRIPTION "Installs files to simplify the integration of the development libraries with CMake")
	set(CPACK_COMPONENT_CMAKE_REQUIRED False)

	set(CPACK_COMPONENT_DOCUMENTATION_DISPLAY_NAME "Documentation")
	set(CPACK_COMPONENT_DOCUMENTATION_DESCRIPTION "Installs the library's official documentation")
	set(CPACK_COMPONENT_DOCUMENTATION_REQUIRED False)

	if(NOT INSTALL_DOC_COMPONENT)
		set(CPACK_COMPONENTS_ALL libraries headers cmake)
	else()
		set(CPACK_COMPONENTS_ALL libraries headers cmake documentation)
	endif()

	if(WIN32 AND NOT UNIX)
		set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
		set(CPACK_NSIS_DISPLAY_NAME "${CPACK_PACKAGE_INSTALL_DIRECTORY} MapGen")
		set(CPACK_NSIS_CONTACT "nathan.giradleau@usherbrooke.ca")
	endif()

	if(APPLE)
		set(CPACK_GENERATOR "productbuild;STGZ;TGZ")
	endif()

	if(UNIX)
		set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local")
	endif()

	include(CPack)
endif()