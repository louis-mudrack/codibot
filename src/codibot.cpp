#include "codibot.h"
#include <dpp/dpp.h>
#include <cstdlib>
#include <atomic>

std::string jokeData;
std::atomic<bool> jokeRequestCompleted(false);

int main()
{
    const char* BOT_TOKEN = std::getenv("BOT_TOKEN");
    
	/* Create bot cluster */
	dpp::cluster bot(BOT_TOKEN);

	/* Output simple log messages to stdout */
	bot.on_log(dpp::utility::cout_logger());

	/* Handle slash command */
	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "joke") {
            bot.request(
                "https://v2.jokeapi.dev/joke/Programming?lang=de&format=txt", dpp::m_get, [&](const dpp::http_request_completion_t& cc) {
                    jokeData = cc.body;
                    jokeRequestCompleted.store(true); // Set the flag to true to indicate completion
                },
                "text/plain"
            );

            // Wait for the request to complete
            while (!jokeRequestCompleted.load()) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }

            // Now, send the response
            event.reply(jokeData);
        }
		else if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}
        else if (event.command.get_command_name() == "timbo") {
			event.reply("WOW! Du hast den geheimen Command gefunden! We stan Tim. Tim is our god.");
		}
        else if (event.command.get_command_name() == "dev") {
			dpp::embed embed = dpp::embed()
                .set_color(dpp::colors::black)
                .set_title("ABOUT MY DEV")
                .set_url("https://github.com/louis-mudrack")
                .set_author("Louis Mudrack", "https://github.com/louis-mudrack", "https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
                .set_description("A developer from germany.")
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
        else if (event.command.get_command_name() == "help") {
            dpp::embed embed = dpp::embed()
                .set_color(dpp::colors::grass_green)
                .set_title("CodiBot | Help Menu")
                .add_field(
                    "Test Bot",
                    "/ping",
                    true
                )
                .add_field(
                    "Get dev information",
                    "/dev",
                    true
                )
                .set_footer(
                    dpp::embed_footer()
                    .set_text("Follow to stay up to date!")
                    .set_icon("https://github.githubassets.com/images/modules/logos_page/GitHub-Mark.png")
                );

            dpp::message msg(event.command.channel_id, embed);
            event.reply(msg);
		}
	});

	bot.on_ready([&bot](const dpp::ready_t& event) {

		if (dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(dpp::slashcommand("ping", "Test if the bot is online!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("joke", "Tells you a joke about programming!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("timbo", "Thats a secret!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("dev", "Send an information about the dev!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("help", "Get help about the bot!", bot.me.id));
		}

		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "C++ and still not knowing what im doing!"));
	});

	bot.start(dpp::st_wait);

	return 0;
}
