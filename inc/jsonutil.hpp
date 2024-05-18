#pragma once 

#include <string>
#include <jsoncpp/json/json.h>

namespace ogm::json{
    Json::Value ReadJson(std::string path){
        std::ifstream ifs;
        ifs.open(path);
        if(!ifs) {
            exit(1);
        }

        std::string jsonStr{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};

        ifs.close();

        Json::Reader reader;
        Json::Value value;
        reader.parse(jsonStr, value);

        for(auto& hoge : value){
            std::cout << hoge << std::endl;
        }

        return value;
    }
    void WriteJson(std::string path, Json::Value value){
        std::ofstream ofs;
        ofs.open(path);
        ofs << value.toStyledString();
        ofs.close();
    }
}