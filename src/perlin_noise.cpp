#include<algorithm>
#include <vector>
#include <random>
#include <numeric>

#include "../include/perlin_noise.h"

Perlin_noise::Perlin_noise(const uint32_t& seed_value = std::default_random_engine::default_seed)
{
    seed(seed_value);
}

Perlin_noise::Perlin_noise()
{
    seed(std::default_random_engine::default_seed);
}

double Perlin_noise::lerp(const double& t, const double& a, const double& b)
{
    return a + t * (b - a);
}

double Perlin_noise::fade(const double& t)
{
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double Perlin_noise::grad(const std::int32_t& hash, const double& x, const double& y, const double& z)
{
    const std::int32_t h = hash & 15;
    const double u = h < 8 ? x : y;
    const double v = h < 4 ? y : h == 12 || h == 14 ? x : z;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

void Perlin_noise::seed(const uint32_t& seed)
{
    permutation.reserve(512);
    permutation.resize(256);

    std::iota(permutation.begin(), permutation.begin() + 256, 0);
    std::default_random_engine engine(seed);
    std::shuffle(permutation.begin(), permutation.begin() + 256, engine);
    permutation.insert(permutation.end(), permutation.begin(), permutation.end());
}

//Output range is between [-0.707, 0,707]
double Perlin_noise::noise(double x, double y, double z) const
{
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

	double res = lerp(w, lerp(v, lerp(u, grad(permutation[AA], x, y, z), grad(permutation[BA], x-1, y, z)), lerp(u, grad(permutation[AB], x, y-1, z), grad(permutation[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(permutation[AA+1], x, y, z-1), grad(permutation[BA+1], x-1, y, z-1)), lerp(u, grad(permutation[AB+1], x, y-1, z-1),	grad(permutation[BB+1], x-1, y-1, z-1))));
	return res;
}

double Perlin_noise::octave_noise(const double& x, const double& y, const double& z, const std::uint32_t& octaves) const
{
    double result = 0.0;
    double amp = 1.0;
    double X = x;
    double Y = y;
    double Z = z;

    for(std::uint32_t i = 0; i < octaves; ++i)
    {
        result += noise(x, y, z) * amp;
        X *= 2.0;
        Y *= 2.0;
        Z *= 2.0;
        amp *= 0.5;
    }
    return result;
}