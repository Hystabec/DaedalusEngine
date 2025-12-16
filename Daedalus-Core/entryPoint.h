#pragma once

#include "src/ddCore.h"

#ifdef DD_PLATFORM_WINDOWS

extern daedalus::Application* daedalus::createApplication(daedalus::ApplicationCommandLineArgs args);

int main(int argc, char** argv)
{
	DD_PROFILE_BEGIN_SESSION("Start up", "Daedalus-Startup");
	daedalus::ApplicationCommandLineArgs CLArgs;
	CLArgs.count = argc;
	CLArgs.args = argv;

	auto app = daedalus::createApplication(CLArgs);
	app->init();
	DD_PROFILE_END_SESSION();

	DD_PROFILE_BEGIN_SESSION("Runtime", "Daedalus-Runtime");
	app->run();
	DD_PROFILE_END_SESSION();

	DD_PROFILE_BEGIN_SESSION("Shutdown", "Daedalus-Shutdown");
	delete app;
	DD_PROFILE_END_SESSION();
}

#endif