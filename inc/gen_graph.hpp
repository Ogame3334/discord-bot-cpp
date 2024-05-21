#pragma once

#include "matplotlibcpp.hpp"
#include "constants.hpp"

namespace ogm::graph
{
    void GenGraphImage(std::string path, std::vector<std::string> x_values, std::vector<double> y_values){
        const auto tategaki = [](std::vector<std::string>& values){
            for(auto& value : values){
                std::string new_value = "";
                for(auto _begin = value.cbegin(); _begin != value.cend() - 1; ++_begin){
                    if((*_begin) == '-') new_value += "/";
                    else new_value += *_begin;
                    new_value += '\n';
                }
                new_value += value.back();
                value = new_value;
            }
        };

        tategaki(x_values);
        matplotlibcpp::figure_size(1280, 720);
        matplotlibcpp::plot(x_values, y_values);
        std::map<std::string, std::string> option{};
        option["fontname"] = "IPAMincho";
        matplotlibcpp::subplots_adjust({{"bottom", 0.2}});
        matplotlibcpp::tick_params({{"labelsize", "8"}});
        matplotlibcpp::title("燃費推移", option);
        matplotlibcpp::xlabel("日付", option);
        matplotlibcpp::ylabel("燃費 [km/L]", option);
        matplotlibcpp::save(path);
    }
    void GenGraph(std::string path){
        auto jsonValue = ogm::json::ReadJson(ogm::constants::GASOLINE_PATH);
        std::vector<std::string> x_values;
        std::vector<double> y_values;
        for(auto& value : jsonValue){
            x_values.push_back(value["date"].asString());
            y_values.push_back(value["distance"].asDouble() / value["gasoline"].asDouble());
        }
        GenGraphImage(path, x_values, y_values);
    }
}
