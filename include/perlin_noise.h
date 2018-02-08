#ifndef DEF_PERLIN_H
#define DEF_PERLIN_H

#include <cstdint>
#include <vector>
namespace mapgen
{
//See http://mrl.nyu.edu/~perlin/noise/
class Perlin_noise
{
public:
    Perlin_noise();
    Perlin_noise(const uint32_t& seed);

    void seed(const uint32_t& seed);

    //Output range is between [-0.707, 0,707]
    double noise(double x, double y, double z) const;
    double octave_noise(const double& x, const double& y, const double& z, const std::uint32_t& octaves, const double& multiplier) const;

private:
    static double lerp(const double& t, const double& a, const double& b);
    static double fade(const double& t);
    static double grad(const std::int32_t& hash, const double& x, const double& y, const double& z);

    std::vector<int32_t> permutation;
};
}
#endif