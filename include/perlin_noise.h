#ifndef DEF_PERLIN_H
#define DEF_PERLIN_H

#include <cstdint>
#include <vector>

//See http://mrl.nyu.edu/~perlin/noise/
class perlin_noise
{
public:
    using seed_type = uint32_t;

    perlin_noise();
    explicit perlin_noise(const seed_type& seed);

    void seed(const seed_type& seed);

    //Output range is between [-0.707, 0,707]
    double noise(double x, double y, double z) const noexcept;
    double octave_noise(const double& x, const double& y, const double& z, const std::uint32_t& octaves, const double& multiplier) const noexcept;

private:
    static double lerp(const double& t, const double& a, const double& b) noexcept;
    static double fade(const double& t) noexcept;
    static double grad(const std::int32_t& hash, const double& x, const double& y, const double& z) noexcept;

    std::vector<int32_t> permutation;
};

#endif