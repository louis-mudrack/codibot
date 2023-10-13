#include "MyBot.h"
#include <dpp/dpp.h>

/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the 
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */
const std::string    BOT_TOKEN    = "MTE2MTk4Njc2MDgyMjIyNjk0NA.GqVOS0.c71-9It1TBMW7hm1t_yVUsVBZrVz2gFTI1aCro";

int main()
{
	/* Create bot cluster */
	dpp::cluster bot(BOT_TOKEN);

	/* Output simple log messages to stdout */
	bot.on_log(dpp::utility::cout_logger());

	/* Handle slash command */
	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
		if (event.command.get_command_name() == "ping") {
			event.reply("Pong!");
		}
        if (event.command.get_command_name() == "timbo") {
			event.reply("WOW! Du hast den geheimen Command gefunden! We stan Tim. Tim is our god.");
		}
		if (event.command.get_command_name() == "dev") {
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
		}		if (event.command.get_command_name() == "help") {
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

	/* Register slash command here in on_ready */
	bot.on_ready([&bot](const dpp::ready_t& event) {
		/* Wrap command registration in run_once to make sure it doesnt run on every full reconnection */
		if (dpp::run_once<struct register_bot_commands>()) {
			bot.global_command_create(dpp::slashcommand("ping", "Test if the bot is online!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("timbo", "Thats a secret!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("dev", "Send an information about the dev!", bot.me.id));
            bot.global_command_create(dpp::slashcommand("help", "Get help about the bot!", bot.me.id));
		}

		/* Set the bot presence as online and "Playing..."! */
		bot.set_presence(dpp::presence(dpp::ps_online, dpp::at_game, "C++ and still not knowing what im doing!"));
	});

	/* Start the bot */
	bot.start(dpp::st_wait);

	return 0;
}
