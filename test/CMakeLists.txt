# Noise texture generator
add_executable(noisetexgen
	noise-texture-generator/main.cpp)
target_include_directories(noisetexgen PUBLIC common "${CMAKE_SOURCE_DIR}/include")
