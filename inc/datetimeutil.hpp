#pragma once

#include <chrono>
#include <sstream>

namespace ogm::datetime
{
    std::time_t GetNow(){
        std::chrono::system_clock::time_point p = std::chrono::system_clock::now();
        return std::chrono::system_clock::to_time_t(p);
    }
    std::string GetToday(){
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%Y-%m-%d");
        return oss.str();
    }
    std::string GetHour(){
        auto now = std::chrono::system_clock::now();
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        std::tm now_tm = *std::localtime(&now_time_t);
        std::ostringstream oss;
        oss << std::put_time(&now_tm, "%H");
        return oss.str();
    }
}