#include <dpp/dpp.h>
// #include "inc/token.hpp"
#include <unistd.h>
#include <chrono>
#include <sstream>
#include <vector>
#include <jsoncpp/json/json.h>
#include <fstream>
#include "inc/jsonutil.hpp"
#include "inc/datetimeutil.hpp"
#include "inc/gen_graph.hpp"
#include "inc/read_image.hpp"
#include "inc/constants.hpp"
#include "inc/bot_move.hpp"

 
int main() {
    bool flag = true;
    std::string preHour = ogm::datetime::GetHour();
    std::string preMonth = ogm::datetime::GetMonth();

    dpp::cluster bot(ogm::constants::BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_ready([&bot](const dpp::ready_t& event) {
        ogm::graph::GenGraph(ogm::constants::FUEL_EFFICIENCY_PATH);
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
                message.add_file(ogm::constants::GASOLINE_JSON_NAME, jsondata.toStyledString(), "application/json");
                event.reply(message);
            }
            else if(subcommand.name == "graph"){
                ogm::graph::GenGraph(ogm::constants::FUEL_EFFICIENCY_PATH);
                dpp::message message;
                auto imageData = ogm::image::readImage(ogm::constants::FUEL_EFFICIENCY_PATH);
                message.add_file(ogm::constants::FUEL_EFFICIENCY_IMAGE_NAME, imageData, "image/png");
                event.reply(message);
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
                if(nowHour == "00"){
                    auto nowMonth = ogm::datetime::GetMonth();
                    if(preMonth != nowMonth){
                        preMonth = nowMonth;
                        dpp::message message{};
                        message.set_channel_id(ogm::constants::id::GASOLINE_BACKUP_CHANNEL_ID);
                        auto jsondata = ogm::json::ReadJson(ogm::constants::GASOLINE_PATH);
                        message.add_file(ogm::constants::GASOLINE_JSON_NAME, jsondata.toStyledString(), "application/json");
                        bot.message_create(message);
                    }
                }
                // std::cout << "日付が変わったよっ！！！！" << std::endl;
                ogm::bot_move::NotifyTask(bot);
            }
        }
        sleep(60);
    }
}
