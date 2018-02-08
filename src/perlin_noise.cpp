#include<algorithm>
#include <vector>
#include <random>
#include <numeric>
#include <iterator>

#include "../include/perlin_noise.hpp"

namespace mapgen {

perlin_noise::perlin_noise(const seed_type &seed_value) noexcept {
    seed(seed_value);
}

perlin_noise::perlin_noise() noexcept {
    seed(std::default_random_engine::default_seed);
}

double perlin_noise::lerp(const double &t, const double &a, const double &b) noexcept {
    return a + t * (b - a);
}

double perlin_noise::fade(const double &t) noexcept {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double perlin_noise::grad(const std::int32_t &hash, const double &x, const double &y, const double &z) noexcept {
    const std::int32_t h = hash & 15;
    const double u = h < 8 ? x : y;
    const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void perlin_noise::seed(const seed_type &seed) noexcept {
    std::iota(std::begin(permutation), std::begin(permutation) + HALF_PERMUTATION_SIZE, 0);

    std::default_random_engine engine(seed);
    std::shuffle(std::begin(permutation), std::begin(permutation) + HALF_PERMUTATION_SIZE, engine);

    // Duplicate first half
    std::copy(permutation.begin(), std::begin(permutation) + HALF_PERMUTATION_SIZE,
              std::begin(permutation) + HALF_PERMUTATION_SIZE);
}

//Output range is between [-0.707, 0,707]

double perlin_noise::raw_noise(double x, double y, double z) const noexcept {
    int X = (int) floor(x) & 255;
    int Y = (int) floor(y) & 255;
    int Z = (int) floor(z) & 255;

    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    double u = fade(x);
    double v = fade(y);
    double w = fade(z);

    int A = permutation[X] + Y;
    int AA = permutation[A] + Z;
    int AB = permutation[A + 1] + Z;
    int B = permutation[X + 1] + Y;
    int BA = permutation[B] + Z;
    int BB = permutation[B + 1] + Z;

    double noise_value = lerp(w,
                              lerp(v,
                                   lerp(u,
                                        grad(permutation[AA], x, y, z),
                                        grad(permutation[BA], x - 1, y, z)),
                                   lerp(u,
                                        grad(permutation[AB], x, y - 1, z),
                                        grad(permutation[BB], x - 1, y - 1, z))),
                              lerp(v,
                                   lerp(u,
                                        grad(permutation[AA + 1], x, y, z - 1),
                                        grad(permutation[BA + 1], x - 1, y, z - 1)),
                                   lerp(u,
                                        grad(permutation[AB + 1], x, y - 1, z - 1),
                                        grad(permutation[BB + 1], x - 1, y - 1, z - 1))));

    //return (noise_value + 7.07) / 1.414;
    return noise_value;
}

double perlin_noise::noise(double x, double y, double z) const noexcept {
    return (raw_noise(x, y, z) + 0.707) / 1.414;
}

double perlin_noise::octave_noise(const double &x, const double &y, const double &z, const std::uint32_t &octaves,
                                  const double &multiplier) const noexcept {
    double total = 0;
    double frequency = 1;
    double amplitude = 1;

    for (int i = 0; i < octaves; i++) {
        total += raw_noise(x * frequency, y * frequency, z * frequency) * amplitude;

        amplitude *= multiplier;
        frequency *= 2;
    }

    return (total + 0.707) / 1.414;
}

}