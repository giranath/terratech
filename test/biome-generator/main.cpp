#include <perlin_noise.h>
#include <helper.hpp>
#include <ppm.h>
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>

std::vector<Perlin_noise> prepare_noises(std::size_t count, uint32_t seed) {
	std::vector<Perlin_noise> noises;
	for(int i = 0; i < 3; ++i) {
		noises.emplace_back(seed + i);
	}

	return noises;
}

int main() {
	// 1. Générer bruit pour l'humidité
	// 2. Générer bruit pour la température
	// 3. Générer bruit pour la magie
	enum {
		HUMIDITY_NOISE,
		TEMPERATURE_NOISE,
		MAGIC_NOISE
	};
	
	std::size_t texture_width = 768;
	std::size_t texture_height = 768;

	uint32_t seed = 51354134;
	auto noises = prepare_noises(3, seed);	

	// generate image
	std::vector<std::vector<rgb>> pixels(texture_width);
	for(std::size_t x = 0; x < texture_width; ++x) {
		pixels[x].reserve(texture_height);
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

			uint8_t color[3];
			std::transform(std::begin(noise_values), std::end(noise_values), std::begin(color), [](double value) -> uint8_t {
					return value * 255;
			});

			pixels[x].emplace_back(color[0], color[1], color[2]);
		}
	}

	// Dump image
	ppm image(pixels);
	std::cout << image << std::endl;
}
