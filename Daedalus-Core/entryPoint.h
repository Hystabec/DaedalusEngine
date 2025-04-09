#pragma once

#include "src/core.h"

#ifdef DD_PLATFORM_WINDOWS

extern daedalusCore::Application* daedalusCore::createApplication();

int main(int argc, char** argv)
{
	DD_PROFILE_BEGIN_SESSION("Start up", "Daedalus-Startup");
	auto app = daedalusCore::createApplication();
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