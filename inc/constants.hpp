#pragma once

#include <dpp/snowflake.h>
#include <string>

namespace ogm
{
namespace constants
{
    const std::string BOT_TOKEN = getenv("DISCORD_BOT_TOKEN");
    const std::string GASOLINE_JSON_NAME = "gasoline.json";
    const std::string GASOLINE_PATH = "../" + GASOLINE_JSON_NAME;
    const std::string FUEL_EFFICIENCY_IMAGE_NAME = "fuel_efficiency.png";
    const std::string FUEL_EFFICIENCY_PATH = "../" + FUEL_EFFICIENCY_IMAGE_NAME;

    namespace id
    {
        const dpp::snowflake TODO_CHANNEL_ID = 1227881613787140097;
        const dpp::snowflake NOTIFY_CHANNEL_ID = 1231795852796887050;
        const dpp::snowflake GASOLINE_BACKUP_CHANNEL_ID = 1242336543667126294;
        const dpp::snowflake OGAME_ID = 239344934262538240;
    }
}
}
