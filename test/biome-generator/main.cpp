#include <perlin_noise.h>
#include <helper.hpp>
#include <ppm.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>

std::vector<Perlin_noise> prepare_noises(std::size_t count, uint32_t seed) {
	std::vector<Perlin_noise> noises;
	for(int i = 0; i < 3; ++i) {
		noises.emplace_back(seed + i);
	}

	return noises;
}

#define CURRENT_HUMIDITY noise_values[HUMIDITY_NOISE]
#define CURRENT_TEMPERATURE noise_values[TEMPERATURE_NOISE]
#define CURRENT_MAGIC noise_values[MAGIC_NOISE]

int main() {
	enum {
		HUMIDITY_NOISE,
		TEMPERATURE_NOISE,
		MAGIC_NOISE
	};

	enum {
		// BIOME_NAME             humidity         | temperature      | magic
		BIOME_JUNGLE,          // 0.75 <= x < 1.00 | 0.50 <= x < 0.65 | 0.00 <= x < 0.75
		BIOME_GRASSLAND,       // 0.25 <= x < 0.75 | 0.00 <= x < 0.50 | 0.00 <= x < 0.75
		BIOME_DESERT,          // 0.00 <= x < 0.25 | 0.65 <= x < 1.00 | 0.00 <= x < 0.75
		BIOME_MAGIC_JUNGLE,    // 0.75 <= x < 1.00 | 0.50 <= x < 0.65 | 0.75 <= x < 1.00
		BIOME_MAGIC_GRASSLAND, // 0.25 <= x < 0.75 | 0.00 <= x < 0.50 | 0.75 <= x < 1.00
		BIOME_MAGIC_DESERT,    // 0.00 <= x < 0.25 | 0.65 <= x < 1.00 | 0.75 <= x < 1.00
		BIOME_SAVANAH,
		BIOME_MAGIC_SAVANAH,
		BIOME_FOREST,
		BIOME_MAGIC_FOREST,
		BIOME_SWAMP,
		BIOME_MAGIC_SWAMP
	};

	std::map<int, rgb> biome_color;
	biome_color[BIOME_JUNGLE] =          rgb(0,   0,   255);
	biome_color[BIOME_GRASSLAND] =       rgb(0,   255, 0);
	biome_color[BIOME_DESERT] =          rgb(255, 0,   0);
	biome_color[BIOME_MAGIC_JUNGLE] =    rgb(255, 0,   255);
	biome_color[BIOME_MAGIC_GRASSLAND] = rgb(255, 255, 0);
	biome_color[BIOME_MAGIC_DESERT] =    rgb(255, 0,   128);
	biome_color[BIOME_SAVANAH] =         rgb(255, 255, 128);
	biome_color[BIOME_MAGIC_SAVANAH] =   rgb(255, 128, 255);
	biome_color[BIOME_FOREST]        =   rgb(128, 200, 0);
	biome_color[BIOME_MAGIC_FOREST]  =   rgb(200, 250, 10);
	biome_color[BIOME_SWAMP]         =   rgb(128, 200, 200);
	biome_color[BIOME_MAGIC_SWAMP]   =   rgb(200, 250, 200);
	
	std::size_t texture_width = 768;
	std::size_t texture_height = 768;

	uint32_t seed = 51354134;
	std::cin >> seed;
	auto noises = prepare_noises(3, seed);	

	// generate biomes
	std::vector<std::vector<int>> biomes(texture_width);
	for(std::size_t x = 0; x < texture_width; ++x) {
		biomes[x].reserve(texture_height);
		for(std::size_t y = 0; y < texture_height; ++y) {
			double noise_values[3];

			for(int i = 0; i < 3; ++i) {
				noise_values[i] = noises[i].octave_noise(static_cast<double>(x) / texture_width,
			                                                 static_cast<double>(y) / texture_height, 
					                                 0.0, 8, 0.5);
			}

			std::transform(std::begin(noise_values), std::end(noise_values), std::begin(noise_values), [](double value) {
				return helper::clamp(helper::normalize(value, -0.707, 0.707), 0.0, 1.0);
			});

			// Ici, on a bruits normalisés entre 0.0 et 1.0
			if(CURRENT_HUMIDITY >= 0.0 && CURRENT_HUMIDITY < 0.25) {
				if(CURRENT_TEMPERATURE >= 0.65 && CURRENT_TEMPERATURE < 1.0) {
					if(CURRENT_MAGIC >= 0.75) {
						biomes[x].push_back(BIOME_MAGIC_DESERT);
					}
					else {
						biomes[x].push_back(BIOME_DESERT);
					}
				}
				else {
					if(CURRENT_MAGIC >= 0.75) {
						biomes[x].push_back(BIOME_MAGIC_SAVANAH);
					}
					else {
						biomes[x].push_back(BIOME_SAVANAH);
					}
				}
			}
			else if(CURRENT_HUMIDITY >= 0.35 && CURRENT_HUMIDITY < 0.75) {
				if(CURRENT_TEMPERATURE >= 0.0 && CURRENT_TEMPERATURE < 0.5) {
					if(CURRENT_MAGIC >= 0.75) {
						biomes[x].push_back(BIOME_MAGIC_GRASSLAND);
					}
					else {
						biomes[x].push_back(BIOME_GRASSLAND);
					}
				}
				else {
					if(CURRENT_MAGIC >= 0.75) {
						biomes[x].push_back(BIOME_MAGIC_FOREST);
					}
					else {
						biomes[x].push_back(BIOME_FOREST);
					}
				}
			}
			else if(CURRENT_HUMIDITY >= 0.75 && CURRENT_HUMIDITY < 1.0) {
				if(CURRENT_TEMPERATURE >= 0.5 && CURRENT_TEMPERATURE < 0.65) {
					if(CURRENT_MAGIC >= 0.75) {
						biomes[x].push_back(BIOME_MAGIC_JUNGLE);
					}
					else {
						biomes[x].push_back(BIOME_JUNGLE);
					}
				}
				else {
					if(CURRENT_MAGIC >= 0.75) {
						biomes[x].push_back(BIOME_MAGIC_SWAMP);
					}
					else {
						biomes[x].push_back(BIOME_SWAMP);
					}
				}
			}

		}
	}

	// Generate images
	std::vector<std::vector<rgb>> pixels(texture_width);
	for(std::size_t x = 0; x < texture_width; ++x) {
		pixels[x].reserve(texture_height);
		for(std::size_t y = 0; y < texture_height; ++y) {
			pixels[x].push_back(biome_color[biomes[x][y]]);
		}
	}

	// Dump image
	ppm image(pixels);
	std::cout << image << std::endl;
}
