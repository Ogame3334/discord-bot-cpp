#pragma once 

#include <string>
#include <jsoncpp/json/json.h>
#include <fstream>

namespace ogm::json{
    Json::Value ReadJson(std::string path){
        std::ifstream ifs;
        ifs.open(path);
        if(!ifs) {
            std::ofstream ofs;
            ofs.open(path);
            ofs << "[]";
            ofs.close();
            ifs.open(path);
        }

        std::string jsonStr{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

        ifs.close();

        Json::Reader reader;
        Json::Value value;
        reader.parse(jsonStr, value);

        return value;
    }
    void WriteJson(std::string path, Json::Value value){
        std::ofstream ofs;
        ofs.open(path);
        ofs << value.toStyledString();
        ofs.close();
    }
}