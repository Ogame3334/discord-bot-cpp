#pragma once

#include <chrono>
#include <sstream>

namespace ogm::datetime
{
    std::tm GetTimeJP(){
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        now_time += 9 * 60 * 60;
        std::tm now_tm = *std::localtime(&now_time);
        return now_tm;
    }
    std::string GetToday(){
        auto now_time = GetTimeJP();
        std::ostringstream oss;
        oss << std::put_time(&now_time, "%Y-%m-%d");
        return oss.str();
    }
    std::string GetHour(){
        auto now_time = GetTimeJP();
        std::ostringstream oss;
        oss << std::put_time(&now_time, "%H");
        return oss.str();
    }
}