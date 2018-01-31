#include "map.h"
#include "perlin_noise.h"
#include "ppm.h"
#include "clump.h"
#include "weighted_chance_structure.h"

#include <helper.hpp>
#include <vector>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <iostream>


int main()
{
    uint32_t seed{};
    std::size_t width{};
    std::size_t height{};

    std::cout << "Enter seed : ";
    std::cin >> seed;
    std::cout << std::endl;

    std::cout << "Enter width : ";
    std::cin >> width;
    std::cout << std::endl;

    std::cout << "Enter heigth : ";
    std::cin >> height;
    std::cout << std::endl;

    Perlin_noise perl(seed);

    std::vector<std::vector<double>> v(width, std::vector<double>(height));

    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            double val = perl.octave_noise(static_cast<double>(j) / width, static_cast<double>(i) / height, 0.0, 8, 0.5);
            val = helper::clamp(helper::normalize(val, -0.707, 0.707), 0.0, 1.0);
            v[i][j] = val;
        }
    }

    Map m(width, height);

    m.add_site_by_noise(0.5, 0, std::less<double>(), v);
    //start with grass color
    std::vector<std::vector<rgb>> image_vector(width, std::vector<rgb>(height, rgb(96, 128, 56)));

    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            //Replace with water color
            if (m.get_region(i, j).has_site(0))
                image_vector[i][j] = rgb(64, 164, 223);
        }
    }
<<<<<<< Updated upstream
    clump c (4, 10, 123);

    std::default_random_engine engine{seed};
    std::uniform_int_distribution<size_t> width_distribution{ 0, width };
    std::uniform_int_distribution<size_t> height_distribution{ 0, height };
    std::vector<point<int>> rando_point(50);
    
    std::for_each(rando_point.begin(), rando_point.end(), [&width_distribution, &height_distribution, &engine](point<int>& p) {
        p = point<int>{ static_cast<int>(width_distribution(engine)), static_cast<int>(height_distribution(engine)) };
    });

    for (auto& p : rando_point)
    {
        std::vector<point<int>> points = c();
        for (auto& clump_p : points)
        {
            if (clump_p.x + p.x >= 0 && clump_p.x + p.x < width && clump_p.y + p.y >= 0 && clump_p.y + p.y < height)
            {
                image_vector[clump_p.x + p.x][clump_p.y + p.y] = rgb(255, 0, 0);
            }
        }
    }
=======

>>>>>>> Stashed changes
    std::ofstream file("water.ppm");
    ppm image(image_vector);
    image.write(file);
}