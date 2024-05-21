#pragma once 

#include <dpp/dpp.h>
#include "constants.hpp"

namespace ogm
{
namespace bot_move
{
    void NotifyTask(dpp::cluster& bot){
        bot.messages_get(ogm::constants::id::TODO_CHANNEL_ID, 0, 0, 0, 20, [&bot](const dpp::confirmation_callback_t& callback) -> void {
                std::string tasks = "<@";
                tasks += ogm::constants::id::OGAME_ID.str();
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
                dpp::message message{ogm::constants::id::NOTIFY_CHANNEL_ID, tasks};
                bot.message_create(message);
            }
        );
    }
}
}
