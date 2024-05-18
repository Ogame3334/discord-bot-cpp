#include <dpp/dpp.h>
#include "inc/token.hpp"
#include <unistd.h>
#include <chrono>
#include <sstream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "inc/jsonutil.hpp"
#include "inc/datetimeutil.hpp"


namespace ogm
{
namespace id
{
    const dpp::snowflake TODO_CHANNEL_ID = 1227881613787140097;
    const dpp::snowflake NOTIFY_CHANNEL_ID = 1231795852796887050;
    const dpp::snowflake OGAME_ID = 239344934262538240;
}
namespace bot_move
{
    void NotifyTask(dpp::cluster& bot){
        bot.messages_get(ogm::id::TODO_CHANNEL_ID, 0, 0, 0, 20, [&bot](const dpp::confirmation_callback_t& callback) -> void {
                std::string tasks = "<@";
                tasks += ogm::id::OGAME_ID.str();
                tasks += ">\nタスクが残ってますよ！！！\n\n## タスク一覧\n";
                for(auto& hoge : callback.get<dpp::message_map>()){
                    bool not_done = true;
                    for(auto& reaction : hoge.second.reactions){
                        if(reaction.emoji_name == "done"){
                            not_done = false;
                            break;
                        }
                    }
                    if(not_done){
                        // tasks.push_back(hoge.second.content);
                        tasks += "- ";
                        tasks += hoge.second.content;
                        tasks += '\n';
                    }
                }
                dpp::message message{ogm::id::NOTIFY_CHANNEL_ID, tasks};
                bot.message_create(message);
            }
        );
    }
    
}
}
 
int main() {
    bool flag = true;
    std::string preHour = ogm::datetime::GetHour();

    dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        std::cout << "ログインしました" << std::endl;
    });

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        std::cout << "content: " << event.msg.content << std::endl;
        if (event.msg.content.find("hoge") != std::string::npos) {
            event.reply("fuga", true);
        }
        if (event.msg.content.find("bad word") != std::string::npos) {
            event.reply("That is not allowed here. Please, mind your language!", true);
        }
        if (event.msg.id == ogm::id::TODO_CHANNEL_ID and event.msg.content.find("hey") == std::string::npos){
            event.reply("hey");
        }
    });

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        } else if (event.command.get_command_name() == "pong") {
            event.reply("Ping!");
        } else if (event.command.get_command_name() == "ding") {
            event.reply("Dong!");
        } else if (event.command.get_command_name() == "dong") {
            event.reply("Ding!");
        }
    });

    // std::cout << ogm::datetime::GetHour() << std::endl;

    // auto temp = ogm::json::ReadJson("gasoline.json");
    // temp[temp.size()] = 100000;
    // ogm::json::WriteJson("test.json", temp);

    bot.start(true);

    //終了まで待機 (8)
    while (flag) {
        auto nowHour = ogm::datetime::GetHour();
        if(preHour != nowHour){
            preHour = nowHour;
            if(nowHour == "00" or nowHour == "08" or nowHour == "20"){
                // std::cout << "日付が変わったよっ！！！！" << std::endl;
                ogm::bot_move::NotifyTask(bot);
            }
        }
        sleep(60);
    }
}
