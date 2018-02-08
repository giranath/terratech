#include "map.h"
#include "perlin_noise.h"
#include "ppm.h"
#include "probability_structure.h"

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

    mapgen::Perlin_noise perl(seed);

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

    mapgen::map m(width, height, seed);
    m.set_biome_by_noise(0.5, 1, std::less<double>(), v);

    mapgen::probability_structure<int16_t> struc;
    struc.set_biome_and_site_occurence(0, { {1,40}, {2,50} });
    struc.set_biome_and_site_occurence(1, { { 3,20 },{ 4,20 } });
    m.generate_by_random_points(100, { {1,2,3,4} }, 400, 400);
    //m.generate_by_elimination(struc);
    //start with grass color
    std::vector<std::vector<rgb>> image_vector(width, std::vector<rgb>(height, rgb(96, 128, 56)));

    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            //Replace with water color
            if (m.get_biome_at(i,j) == 1)
                image_vector[i][j] = rgb(64, 164, 223);
            if (m.has_site_at(i, j,1))
                image_vector[i][j] = rgb(0, 0, 255);
            if (m.has_site_at(i, j, 2))
                image_vector[i][j] = rgb(0, 255, 0);
            if (m.has_site_at(i, j, 3))
                image_vector[i][j] = rgb(255, 0, 0);
            if (m.has_site_at(i, j, 4))
                image_vector[i][j] = rgb(255, 255, 255);
        }
    }
    
  
    std::ofstream file("water.ppm");
    ppm image(image_vector);
    image.write(file);
}