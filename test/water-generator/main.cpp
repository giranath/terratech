#include "map.h"
#include "perlin_noise.h"
#include "ppm.h"
#include "ressource_generator.h"

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

    map m(width, height);
    
    ressource_generator ressource_gen{m.regions};
    m.set_biome_by_noise(0.5, 1, std::less<double>(), v);

    //ressource_gen.generate_by_random_points(500, { 2,3,4 },width,height);
    ressource_gen.generate_by_weight_and_biome({ {0,{{0,20}, {1,0.5},{ 2,1 } }},{ 1,{ { 0,20 },{ 3,0.5 },{ 4,1 } } } });
    
    //start with grass color
    std::vector<std::vector<rgb>> image_vector(width, std::vector<rgb>(height, rgb(96, 128, 56)));

    for (size_t i = 0; i < width; ++i)
    {
        for (size_t j = 0; j < height; ++j)
        {
            //Replace with water color
            if (m.get_region(i, j).get_biome() == 1)
                image_vector[i][j] = rgb(64, 164, 223);
            if (m.get_region(i, j).has_site(1))
                image_vector[i][j] = rgb(0, 0, 255);
            if (m.get_region(i, j).has_site(2))
                image_vector[i][j] = rgb(0, 255, 0);
            if (m.get_region(i, j).has_site(3))
                image_vector[i][j] = rgb(255, 0, 0);
            if (m.get_region(i, j).has_site(4))
                image_vector[i][j] = rgb(255, 255, 255);
        }
    }
    
  
    std::ofstream file("water.ppm");
    ppm image(image_vector);
    image.write(file);
}