#ifndef DEF_PPM_H
#define DEF_PPM_H

#include <vector>
#include <sstream>
#include <fstream>

#include "rgb.h"

struct ppm
{
    ppm(const std::vector < std::vector<rgb>>& data) : data(data), heigth(data.front().size()), width(data.size()) {}
    size_t width;
    size_t heigth;
    std::vector < std::vector<rgb>> data;

    void write(std::string path)
    {
        std::ofstream pgmFile;
        std::ostringstream convert;

        pgmFile.open(path);

        std::string header{};
        std::string data_value{};

        convert << "P3 \n";
        convert << width;
        convert << " ";
        convert << heigth;
        convert << " \n 255 \n";
        header = convert.str();

        for (auto& col : data)
        {
            for (auto& row : col)
            {
                convert.str("");
                convert.clear();
                convert << static_cast<int>(row.r);
                convert << " ";
                convert << static_cast<int>(row.g);
                convert << " ";
                convert << static_cast<int>(row.b);
                convert << " ";
                data_value += convert.str();
            }
            data_value += " \n";
        }
        pgmFile << header + data_value;
    }
};
#endif