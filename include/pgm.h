#ifndef DEF_PGM_H
#define DEF_PGM_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <sstream>


struct pgm
{
    pgm(const std::vector < std::vector<uint8_t>>& data) : data(data), heigth(data.front().size()), width(data.size()) {}
    size_t width;
    size_t heigth;
    std::vector < std::vector<uint8_t>> data;

    void write(std::string path)
    {
        std::ofstream pgmFile;
        std::ostringstream convert;

        pgmFile.open(path);

        std::string header{};
        std::string data_value{};

        convert << "P2 \n";
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

                convert << static_cast<int>(row);
                data_value += convert.str() + " ";
            }
            data_value += " \n";
        }
        pgmFile << header + data_value;
    }
};



#endif