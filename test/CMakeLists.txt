# Noise texture generator
add_executable(noisetexgen
	noise-texture-generator/main.cpp)
target_include_directories(noisetexgen PUBLIC "common" "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(noisetexgen mapgen)

add_executable(biometexgen
	biome-generator/main.cpp)
target_include_directories(biometexgen PUBLIC "common" "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(biometexgen mapgen)

add_executable(watertexgen
	water-generator/main.cpp)
target_include_directories(watertexgen PUBLIC "common" "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(watertexgen mapgen)
