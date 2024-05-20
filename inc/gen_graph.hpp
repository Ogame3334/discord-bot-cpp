#pragma once

#include "matplotlibcpp.hpp"

namespace ogm::graph
{
    void GenGraph(std::string path){
        namespace plt = matplotlibcpp;
        std::vector<std::string> x_values{"2024-05", "2024-06", "2024-07", "2024-08", "2024-09"};
        std::vector<double> y_values{10, 4, 8, 15, 10};
        plt::plot(x_values, y_values);
        std::map<std::string, std::string> option{};
        option["fontname"] = "IPAMincho";
        plt::title("燃費推移", option);
        plt::xlabel("日付", option);
        plt::ylabel("燃費 [km/L]", option);
        plt::save(path);
    }
}
