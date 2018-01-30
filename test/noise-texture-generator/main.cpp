#include <pgm.h>
#include <helper.hpp>
#include <perlin_noise.h>
#include <mapgen_config.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>

#ifndef _WIN32

#include <stdlib.h>
#include <unistd.h>

#endif

static const char *VERSION = "1.0.0";

struct Arguments {
    bool read_seed_from_stdin = false;
    uint32_t seed = 0;
    std::size_t width = 1024;
    std::size_t height = 1024;
    uint32_t octaves = 8;
    double multiplier = 1.0;

    Arguments() = default;
};


void show_help(const char *exec) {
    std::cout << "usage: " << exec << "[OPTION]..." << std::endl;
    std::cout << "generate a noise texture to standard output.\n" << std::endl;

    std::cout << "options:" << std::endl;
    std::cout << "  -W <width>      Set the texture's width in pixels  [1024]" << std::endl;
    std::cout << "  -H <height>     Set the texture's height in pixels [1024]" << std::endl;
    std::cout << "  -o <octaves>    Set the perlin noise's octaves     [8]" << std::endl;
    std::cout << "  -m <multiplier> Set the noise's multiplier         [1.0]" << std::endl;
    std::cout << "  -s <seed>|-     Use the specified seed to generate the noise" << std::endl;
    std::cout << "                  if - is used, the seed will be read from stdin" << std::endl;
    std::cout << "  -v              Display the version" << std::endl;
    std::cout << "  -h              Display this help" << std::endl;
}

void show_version() {
    std::cout << "noisetexgen (mapgen " MAPGEN_VERSION_STR ") " << VERSION << std::endl;
}

Arguments parse_arguments(int argc, char *const argv[]) {
    Arguments args;

#ifndef _WIN32
    int c;
    while ((c = getopt(argc, argv, "+hvW:H:o:m:s:")) != -1) {
        switch (c) {
            case 'h': // help
                show_help(argv[0]);
                exit(0);
                break;
            case 'v': // version
                show_version();
                exit(0);
                break;
            case 'W': // Width
                args.width = std::atoi(optarg);
                break;
            case 'H': // Height
                args.height = std::atoi(optarg);
                break;
            case 'o': // octaves
                args.octaves = std::atoi(optarg);
                break;
            case 'm': // multiplier
                args.multiplier = std::atof(optarg);
                break;
            case 's': // seed
                if (std::strcmp(optarg, "-") == 0) {
                    args.read_seed_from_stdin = true;
                } else {
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

int main(int argc, char *argv[]) {
    Arguments args = parse_arguments(argc, argv);

    // Generate noise
    perlin_noise noise;

    if (args.read_seed_from_stdin) {
        uint32_t seed;
        std::cin >> seed;
        noise.seed(seed);
    } else {
        noise.seed(args.seed);
    }

    // Generate image
    std::vector<std::vector<uint8_t>> pixels(args.width);
    for (auto &i: pixels) {
        i.resize(args.height);
    }

    for (std::size_t x = 0; x < args.width; ++x) {
        for (std::size_t y = 0; y < args.height; ++y) {
            double noise_value = noise.octave_noise(static_cast<double>(x) / args.width,
                                                    static_cast<double>(y) / args.height,
                                                    0.0,
                                                    args.octaves,
                                                    args.multiplier);
            pixels[x][y] = static_cast<uint8_t>(helper::clamp(helper::normalize(noise_value, -0.707, 0.707), 0.0, 1.0) *
                                                255.0);
        }
    }

    // Dump image
    pgm image(pixels);
    std::cout << image << std::endl;
}
