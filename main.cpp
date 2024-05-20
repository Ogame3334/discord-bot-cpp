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
#include "inc/gen_graph.hpp"


namespace ogm
{
namespace id
{
    const dpp::snowflake TODO_CHANNEL_ID = 1227881613787140097;
    const dpp::snowflake NOTIFY_CHANNEL_ID = 1231795852796887050;
    const dpp::snowflake OGAME_ID = 239344934262538240;
}
namespace constants
{
    const std::string GASOLINE_PATH = "../gasoline.json";
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
        ogm::graph::GenGraph("../hoge.png");
        std::cout << "bot is ready." << std::endl;
        ogm::json::ReadJson(ogm::constants::GASOLINE_PATH);
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_bulk_command_delete();
            dpp::slashcommand gasoline("gasoline", "ガソリンの記録", bot.me.id);
            gasoline.add_option(
                dpp::command_option(dpp::co_sub_command, "register", "ガソリンと距離を記録します")
            )
            .add_option(
                dpp::command_option(dpp::co_sub_command, "json", "記録のjsonファイルを返します")
            )
            .add_option(
                dpp::command_option(dpp::co_sub_command, "graph", "燃費の折れ線グラフを返します")
            );

            bot.global_command_create(gasoline);
        }
    });

    // bot.on_message_create([&bot](const dpp::message_create_t& event) {
    //     if (event.msg.content.find("hoge") != std::string::npos) {
    //         event.reply("fuga", true);
    //     }
    //     if (event.msg.content.find("bad word") != std::string::npos) {
    //         event.reply("That is not allowed here. Please, mind your language!", true);
    //     }
    //     if (event.msg.id == ogm::id::TODO_CHANNEL_ID and event.msg.content.find("hey") == std::string::npos){
    //         event.reply("hey");
    //     }
    // });

    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        dpp::command_interaction cmd_data = event.command.get_command_interaction();
        if (event.command.get_command_name() == "gasoline") {
            auto subcommand = cmd_data.options[0];
            if(subcommand.name == "register"){
                /* Instantiate an interaction_modal_response object */
                dpp::interaction_modal_response modal("my_modal", "Please enter stuff");
    
                /* Add a text component */
                modal.add_component(
                    dpp::component()
                        .set_label("走った距離km")
                        .set_id("field_id")
                        .set_type(dpp::cot_text)
                        .set_placeholder("512.1")
                        .set_min_length(1)
                        .set_max_length(20)
                        .set_text_style(dpp::text_short)
                );
    
                /* Add another text component in the next row, as required by Discord */
                modal.add_row();
                modal.add_component(
                    dpp::component()
                        .set_label("ガソリンL")
                        .set_id("field_id2")
                        .set_type(dpp::cot_text)
                        .set_placeholder("25.6")
                        .set_min_length(1)
                        .set_max_length(20)
                        .set_text_style(dpp::text_short)
                );
    
                /* Trigger the dialog box. All dialog boxes are ephemeral */
                event.dialog(modal);
            }
            else if(subcommand.name == "json"){
                dpp::message message;
                auto jsondata = ogm::json::ReadJson(ogm::constants::GASOLINE_PATH);
                message.add_file(ogm::constants::GASOLINE_PATH, jsondata.toStyledString(), "application/json");
                event.reply(message);
            }
            else if(subcommand.name == "graph"){
                event.reply("いつかグラフを返すようにするよ！！");
            }
        } 
    });

    bot.on_form_submit([](const dpp::form_submit_t & event) {
        /* For this simple example, we know the first element of the first row ([0][0]) is value type string.
         * In the real world, it may not be safe to make such assumptions!
         */
        std::string d = std::get<std::string>(event.components[0].components[0].value);
        std::string g = std::get<std::string>(event.components[1].components[0].value);
        double distance = std::stod(d);
        double gasoline = std::stod(g);
 
        dpp::message m;
        std::ostringstream oss;
        oss << "走行距離：" << distance << "[km]" << std::endl;
        oss << "ガソリン：" << gasoline << "[L]" << std::endl;
        oss << "　　燃費：" << distance / gasoline << "[km/L]" << std::endl;

        auto jsondata = ogm::json::ReadJson(ogm::constants::GASOLINE_PATH);
        Json::Value value;
        value["date"] = ogm::datetime::GetToday();
        value["gasoline"] = gasoline;
        value["distance"] = distance;
        std::cout << jsondata.size() << std::endl;
        jsondata[jsondata.size()] = value;
        ogm::json::WriteJson(ogm::constants::GASOLINE_PATH, jsondata);


        m.set_content(oss.str());//.set_flags(dpp::m_ephemeral);
 
        /* Emit a reply. Form submission is still an interaction and must generate some form of reply! */
        event.reply(m);
    });


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
