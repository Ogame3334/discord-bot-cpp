#include <dpp/dpp.h>
#include "inc/token.hpp"
#include <unistd.h>
#include <boost/asio.hpp>
#include <chrono>
#include <sstream>

namespace channel_id
{
    const dpp::snowflake TODO_CHANNEL_ID = 1227881613787140097;
    const dpp::snowflake NOTIFY_CHANNEL_ID = 1231795852796887050;
}

namespace ogm
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
namespace bot_move
{
    void NotifyTask(){
        bot.messages_get(1227881613787140097, 0, 0, 0, 20, [](const dpp::confirmation_callback_t& callback){
        for(auto& hoge : callback.get<dpp::message_map>()){
            std::cout << hoge.first << ": " << hoge.second.content << std::endl;
            for(auto& reaction : hoge.second.reactions){
                std::cout << reaction.emoji_name << std::endl;
            }
        }
        });
    }
}
}
 
int main() {
    bool flag = true;
    std::string yesterday = ogm::GetToday();

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        std::cout << "ログインしました" << std::endl;
    });

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        /* See if the message contains the phrase we want to check for.
         * If there's at least a single match, we reply and say it's not allowed.
         */
        std::cout << "content: " << event.msg.content << std::endl;
        if (event.msg.content.find("hoge") != std::string::npos) {
            event.reply("fuga", true);
        }
        if (event.msg.content.find("bad word") != std::string::npos) {
            event.reply("That is not allowed here. Please, mind your language!", true);
        }
        if (event.msg.channel_id == channel_id::TODO_CHANNEL_ID and event.msg.content.find("hey") == std::string::npos){
            event.reply("hey");
        }
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }
    });

    

    bot.start(true);

    //終了まで待機 (8)
    while (flag) {
        auto today = ogm::GetToday();
        if(yesterday != today){

        }
        sleep(60);
    }
}
