#include <iostream>
#include <csignal>
#include <thread>
#include <chrono>
#include "discord-files/discord.h"

struct DiscordState {
	std::unique_ptr<discord::Core> core;
};

namespace {
	volatile bool interrupted{ false };
}

int main()
{
	DiscordState state{};
	discord::Core* core{};
	auto response = discord::Core::Create(461618159171141643, DiscordCreateFlags_Default, &core);
	state.core.reset(core);

	if (!state.core) {
		std::cout << "Failed to instantiate Discord!";
		std::exit(-1);
	}

	discord::Activity activity{};
	activity.SetDetails("Fruit Tarts");
	activity.SetState("Pop Snacks");
	activity.GetAssets().SetSmallImage("the");
	activity.GetAssets().SetSmallText("i mage");
	activity.GetAssets().SetLargeImage("the");
	activity.GetAssets().SetLargeText("u mage");
	activity.SetType(discord::ActivityType::Playing);
	state.core->ActivityManager().UpdateActivity(activity, [](discord::Result result) {
		std::cout << ((result == discord::Result::Ok) ? "Succeeded" : "Failed")
			<< " updating activity!\n";
	});


	std::signal(SIGINT, [](int) {
		interrupted = true;
	});
	do {
		state.core->RunCallbacks();
		std::this_thread::sleep_for(std::chrono::milliseconds(16));
	} while (!interrupted);
}
