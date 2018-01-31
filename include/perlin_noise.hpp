#ifndef DEF_MAPGEN_PERLIN_HPP
#define DEF_MAPGEN_PERLIN_HPP

#include <cstdint>
#include <array>

namespace mapgen {

//See http://mrl.nyu.edu/~perlin/noise/
class perlin_noise {
public:
    using seed_type = uint32_t;

    perlin_noise() noexcept;

    explicit perlin_noise(const seed_type &seed) noexcept;

    void seed(const seed_type &seed) noexcept;

    //Output range is between [-0.707, 0,707]
    double raw_noise(double x, double y, double z) const noexcept;

    //Output range is between [0.0, 1.0]
    double noise(double x, double y, double z) const noexcept;

    //Output range is between [0.0, 1.0]
    double octave_noise(const double &x, const double &y, const double &z, const std::uint32_t &octaves,
                        const double &multiplier) const noexcept;

private:
    static const std::size_t PERMUTATION_SIZE = 512;
    static const std::size_t HALF_PERMUTATION_SIZE = PERMUTATION_SIZE / 2;

    static double lerp(const double &t, const double &a, const double &b) noexcept;

    static double fade(const double &t) noexcept;

    static double grad(const std::int32_t &hash, const double &x, const double &y, const double &z) noexcept;

    std::array<int32_t, PERMUTATION_SIZE> permutation;
};

}

#endif