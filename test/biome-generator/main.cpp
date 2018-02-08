#include <perlin_noise.h>
#include <biome_table.hpp>
#include <mapgen_config.h>
#include <helper.hpp>
#include <ppm.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <map>
#include <type_traits>

#ifndef _WIN32
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#endif

static const char* VERSION = "1.0.0";

// Names the different noises uses to generate biomes
// In this world, a biome is defined by the humidity and the temperature of a region
enum {
	HUMIDITY_NOISE,
	TEMPERATURE_NOISE,
	NOISES_COUNT
};

// Enumerates every supported biomes
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

std::vector<mapgen::Perlin_noise> prepare_noises(std::size_t count, uint32_t seed);
mapgen::biome_table create_biomes_table();
std::map<int, rgb> create_biome_color_mapping();

struct Arguments {
	std::size_t width = 768;
	std::size_t height = 768;
	uint32_t seed = 0;
	bool seed_from_stdin = false;

	Arguments() = default;
};

void show_help(const char* exec) {
	std::cout << "usage: " << exec << "[OPTION]..." << std::endl;
	std::cout << "generate a biome distribution texture to standard output.\n" << std::endl;

	std::cout << "options:" << std::endl;
	std::cout << "  -W <width>      Set the texture's width in pixels  [768]" << std::endl;
	std::cout << "  -H <height>     Set the texture's height in pixels [768]" << std::endl;
	std::cout << "  -s <seed>|-     Use the specified seed to generate the noise" << std::endl; 
	std::cout << "                  if - is used, the seed will be read from stdin" << std::endl;
	std::cout << "  -v              Display the version" << std::endl;
	std::cout << "  -h              Display this help" << std::endl;
}

void show_version() {
	std::cout << "biometexgen (mapgen " MAPGEN_VERSION_STR ") " << VERSION << std::endl;
}

Arguments parse_arguments(int argc, char* const argv[]) {
	Arguments args;

#ifndef _WIN32
	int c;
	while((c = getopt(argc, argv, "+hvW:H:s:")) != -1) {
		switch(c) {
			case 'h':
				show_help(argv[0]);
				exit(0);
				break;
			case 'v':
				show_version();
				exit(0);
				break;
			case 'W':
				args.width = std::atoi(optarg);
				break;
			case 'H':
				args.height = std::atoi(optarg);
				break;
			case 's':
				if(std::strcmp(optarg, "-") == 0) {
					args.seed_from_stdin = true;
				}
				else {
					args.seed = std::atoi(optarg);
				}
				break;
			default:
				show_help(argv[0]);
				exit(1);
				break;
		}
	}
#endif

	return args;
}

int main(int argc, char* argv[]) {
	auto args = parse_arguments(argc, argv);

	std::size_t texture_width = args.width;
	std::size_t texture_height = args.height;

	uint32_t seed = args.seed;
	if(args.seed_from_stdin) {
		std::cin >> seed;
	}

	auto biome_rep = create_biomes_table();
	auto biome_color = create_biome_color_mapping();
	auto noises = prepare_noises(NOISES_COUNT, seed);

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

			// TODO: The noise should normalize the value
			std::transform(std::begin(noise_values), std::end(noise_values), std::begin(noise_values), [](double value) {
				return helper::clamp(helper::normalize(value, -0.707, 0.707), 0.0, 1.0);
			});

			const double CURRENT_HUMIDITY = noise_values[HUMIDITY_NOISE];
			const double CURRENT_TEMPERATURE = noise_values[TEMPERATURE_NOISE];

			// Here we use the biome repartition table to choose a biome for a specific point of the map
			biomes[x].push_back(biome_rep.biome_with(CURRENT_HUMIDITY, CURRENT_TEMPERATURE));
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
   std::ofstream file("test.ppm");
   image.write(file);
}

std::vector<mapgen::Perlin_noise> prepare_noises(std::size_t count, uint32_t seed) {
	std::vector<mapgen::Perlin_noise> noises;
	for(std::size_t i = 0; i < count; ++i) {
		noises.emplace_back(seed + i);
	}

	return noises;
}

mapgen::biome_table create_biomes_table() {
	// Here we create a biome repartition table
	// 1.0 ^
	//     |  +---------+---------+---------+---------+---------+---------+
	//     H  |              FOREST         |  SWAMP  |    RAIN FOREST    |
	//     U  +---------+---------+         +---------+---------+---------+
	//     M  |                   |                   |  SEASONAL FOREST  |
	//     I  +---------+         +---------+---------+---------+---------+
	//     D  |         |  TAIGA  |       WOODS       |      SAVANNA      |
	//     I  +         +---------+---------+---------+---------+---------+
	//     T  |       TUNDRA      |    GRASS DESERT   |       DESERT      |
	//     Y  +---------+---------+---------+---------+---------+---------+
	//     |
	//     +----------------------- TEMPERATURE --------------------------->
	// 0.0                                                             1.0
	//
	mapgen::biome_table biome_rep(6,4);
	biome_rep.set_biome_at(0, 0, BIOME_TUNDRA);
	biome_rep.set_biome_at(1, 0, BIOME_TUNDRA);
	biome_rep.set_biome_at(2, 0, BIOME_GRASS_DESERT);
	biome_rep.set_biome_at(3, 0, BIOME_GRASS_DESERT);
	biome_rep.set_biome_at(4, 0, BIOME_DESERT);
	biome_rep.set_biome_at(5, 0, BIOME_DESERT);

	biome_rep.set_biome_at(0, 1, BIOME_TUNDRA);
	biome_rep.set_biome_at(1, 1, BIOME_TAIGA);
	biome_rep.set_biome_at(2, 1, BIOME_WOODS);
	biome_rep.set_biome_at(3, 1, BIOME_WOODS);
	biome_rep.set_biome_at(4, 1, BIOME_SAVANNA);
	biome_rep.set_biome_at(5, 1, BIOME_SAVANNA);

	biome_rep.set_biome_at(0, 2, BIOME_TAIGA);
	biome_rep.set_biome_at(1, 2, BIOME_TAIGA);
	biome_rep.set_biome_at(2, 2, BIOME_FOREST);
	biome_rep.set_biome_at(3, 2, BIOME_FOREST);
	biome_rep.set_biome_at(4, 2, BIOME_SEASONAL_FOREST);
	biome_rep.set_biome_at(5, 2, BIOME_SEASONAL_FOREST);

	biome_rep.set_biome_at(0, 3, BIOME_FOREST);
	biome_rep.set_biome_at(1, 3, BIOME_FOREST);
	biome_rep.set_biome_at(2, 3, BIOME_FOREST);
	biome_rep.set_biome_at(3, 3, BIOME_SWAMP);
	biome_rep.set_biome_at(4, 3, BIOME_RAIN_FOREST);
	biome_rep.set_biome_at(5, 3, BIOME_RAIN_FOREST);

	return biome_rep;
}

std::map<int, rgb> create_biome_color_mapping() {
	// Here we asign a color to a biome
	// see https://i.stack.imgur.com/vlvQQ.png for color code
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

	return biome_color;
}
