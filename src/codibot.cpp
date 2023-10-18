#include "codibot.h"
#include <dpp/dpp.h>
#include <cstdlib>
#include <atomic>

std::string jokeData;
std::atomic<bool> jokeRequestCompleted(false);

void jokeCommand(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    bot.request("https://v2.jokeapi.dev/joke/Programming?lang=de&format=txt", dpp::m_get,
        [&](const dpp::http_request_completion_t& cc) {
            jokeData = cc.body;
            jokeRequestCompleted.store(true);
        },
        "text/plain"
    );

    while (!jokeRequestCompleted.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    event.reply(jokeData);
}

void pingCommand(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    event.reply("Pong!");
}

void devCommand(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::black)
        .set_title("ABOUT MY DEV")
        .set_url("https://github.com/louis-mudrack")
        .set_author("Louis Mudrack", "https://github.com/louis-mudrack", "https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
        .set_description("A developer from Germany.")
        .set_thumbnail("https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
        .set_image("https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
        .set_footer(
            dpp::embed_footer()
            .set_text("Follow to stay up to date!")
            .set_icon("https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
        )
        .set_timestamp(time(0));

    dpp::message msg(event.command.channel_id, embed);
    event.reply(msg);
}

void helpCommand(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    dpp::embed embed = dpp::embed()
        .set_color(dpp::colors::grass_green)
        .set_title("CodiBot | Help Menu")
        .add_field("Test Bot", "/ping", true)
        .add_field("Get dev information", "/dev", true)
        .set_footer(
            dpp::embed_footer()
            .set_text("Follow to stay up to date!")
            .set_icon("https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
        );

    dpp::message msg(event.command.channel_id, embed);
    event.reply(msg);
}

void handleSlashCommand(dpp::cluster& bot, const dpp::slashcommand_t& event) {
    const std::string commandName = event.command.get_command_name();

    if (commandName == "joke") {
        jokeCommand(bot, event);
    }
    else if (commandName == "ping") {
        pingCommand(bot, event);
    }
    else if (commandName == "dev") {
        devCommand(bot, event);
    }
    else if (commandName == "help") {
        helpCommand(bot, event);
    }
    else {
        // Handle unknown command
        event.reply("Unknown command.");
    }
}

void registerCommands(dpp::cluster& bot) {
    bot.global_command_create(dpp::slashcommand("ping", "Test if the bot is online!", bot.me.id));
    bot.global_command_create(dpp::slashcommand("joke", "Tells you a joke about programming!", bot.me.id));
    bot.global_command_create(dpp::slashcommand("dev", "Send information about the dev!", bot.me.id));
    bot.global_command_create(dpp::slashcommand("help", "Get help about the bot!", bot.me.id));
}

int main() {
    const char* BOT_TOKEN = std::getenv("BOT_TOKEN");

    dpp::cluster bot(BOT_TOKEN);

    bot.on_log(dpp::utility::cout_logger());

    bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        handleSlashCommand(bot, event);
    });

    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            registerCommands(bot);
        }

        bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "C++ and still not knowing what I'm doing!"));
    });

    bot.start(dpp::st_wait);

    return 0;
}
