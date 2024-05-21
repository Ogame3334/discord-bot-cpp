#pragma once

#include <string>
#include <fstream>

namespace ogm
{
namespace image
{
    std::string readImage(std::string path){
        std::ifstream ifs;
        ifs.open(path);

        if(!ifs){
            return "";
        }
        
        std::string imageData{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

        ifs.close();

        return imageData;
    }
}
}
