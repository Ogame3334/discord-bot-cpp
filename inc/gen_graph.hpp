#pragma once

#include "matplotlibcpp.hpp"

namespace ogm::graph
{
    void GenGraph(std::string path){
        using plt = matplotlibcpp;
        std::vector<double> x{ 1, 2, 3, 4 };
        std::vector<double> y{ 3, 4, 6, 8 };

        std::map<std::string, std::string> option{};
        option["fontname"] = "IPAMincho";
        plt::title("ほげほげ", option);
        
        plt::plot({1, 2, 4, 8, 16});
        plt::save(path);
    }
}
