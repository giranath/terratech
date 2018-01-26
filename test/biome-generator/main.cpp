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

int main() {
	enum {
		HUMIDITY_NOISE,
		TEMPERATURE_NOISE,
		NOISES_COUNT
	};

	enum {
		BIOME_RAIN_FOREST,
		BIOME_SWAMP,
		BIOME_SEASONAL_FOREST,
		BIOME_FOREST,
		BIOME_TAIGA,
		BIOME_WOODS,
		BIOME_SAVANNA,
		BIOME_DESERT,
		BIOME_GRASS_DESERT,
		BIOME_TUNDRA,
		BIOME_COUNT
	};

	// Set the color for each biomes
	std::map<int, rgb> biome_color;
	biome_color[BIOME_RAIN_FOREST] =     rgb(0,   0,   255);
	biome_color[BIOME_SWAMP] =           rgb(63,  64,  255);
	biome_color[BIOME_SEASONAL_FOREST] = rgb(170, 0,   255);
	biome_color[BIOME_FOREST] =          rgb(191, 64,  255);
	biome_color[BIOME_TAIGA] =           rgb(255, 128, 255);
	biome_color[BIOME_WOODS] =           rgb(255, 64,  191);
	biome_color[BIOME_SAVANNA] =         rgb(255, 0,   170);
	biome_color[BIOME_DESERT] =          rgb(255, 0,   0);
	biome_color[BIOME_GRASS_DESERT] =    rgb(255, 97,  97);
	biome_color[BIOME_TUNDRA] =          rgb(255, 191, 212);

	std::size_t texture_width = 768;
	std::size_t texture_height = 768;

	uint32_t seed = 51354134;
	std::cin >> seed;
	auto noises = prepare_noises(2, seed);

	// generate biomes
	std::vector<std::vector<int>> biomes(texture_width);
	for(std::size_t x = 0; x < texture_width; ++x) {
		biomes[x].reserve(texture_height);
		for(std::size_t y = 0; y < texture_height; ++y) {
			double noise_values[NOISES_COUNT];
			for(int i = 0; i < 2; ++i) {
				noise_values[i] = noises[i].octave_noise(static_cast<double>(x) / texture_width,
				                                         static_cast<double>(y) / texture_height, 
				                                         0.0, 8, 0.5);
			}

			std::transform(std::begin(noise_values), std::end(noise_values), std::begin(noise_values), [](double value) {
				return helper::clamp(helper::normalize(value, -0.707, 0.707), 0.0, 1.0);
			});

			if(CURRENT_HUMIDITY >= 0 && CURRENT_HUMIDITY < 0.25 
			   && CURRENT_TEMPERATURE <= 1.0 && CURRENT_TEMPERATURE > 0.6) {
				biomes[x].push_back(BIOME_DESERT);
			}
			else if(CURRENT_HUMIDITY >= 0 && CURRENT_HUMIDITY < 0.25
			        && CURRENT_TEMPERATURE <= 0.6 && CURRENT_TEMPERATURE > 0.25){
				biomes[x].push_back(BIOME_GRASS_DESERT);
			}
			else if(CURRENT_HUMIDITY >= 0 && CURRENT_HUMIDITY < 0.5
			        && CURRENT_TEMPERATURE <= 0.25) {
			    biomes[x].push_back(BIOME_TUNDRA);
			}
			else if(CURRENT_HUMIDITY >= 0.25 && CURRENT_HUMIDITY < 0.5
			        && CURRENT_TEMPERATURE <= 1.0 && CURRENT_TEMPERATURE > 0.75) {
			    biomes[x].push_back(BIOME_SAVANNA);
			}
			else if(CURRENT_HUMIDITY >= 0.25 && CURRENT_HUMIDITY < 0.5
			        && CURRENT_TEMPERATURE <= 0.75 && CURRENT_TEMPERATURE > 0.5) {
			    biomes[x].push_back(BIOME_WOODS);
			}
			else if(CURRENT_HUMIDITY >= 0.25 && CURRENT_HUMIDITY < 0.75
			        && CURRENT_TEMPERATURE <= 0.5 && CURRENT_TEMPERATURE > 0.25) {
			    biomes[x].push_back(BIOME_TAIGA);
			}
			else if(CURRENT_HUMIDITY >= 0.5 && CURRENT_HUMIDITY < 0.75
			        && CURRENT_TEMPERATURE <= 1.0 && CURRENT_TEMPERATURE > 0.75) {
			    biomes[x].push_back(BIOME_SEASONAL_FOREST);
			}
			else if(CURRENT_HUMIDITY >= 0.5 && CURRENT_HUMIDITY < 0.75
			        && CURRENT_TEMPERATURE <= .75 && CURRENT_TEMPERATURE > 0.5) {
			    biomes[x].push_back(BIOME_FOREST);
			}
			else if(CURRENT_HUMIDITY >= 0.75 && CURRENT_HUMIDITY < 1.0
			        && CURRENT_TEMPERATURE <= .75 && CURRENT_TEMPERATURE > 0.5) {
			    biomes[x].push_back(BIOME_SWAMP);
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
