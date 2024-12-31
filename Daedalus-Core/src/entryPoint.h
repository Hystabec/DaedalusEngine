#pragma once

#ifdef DD_PLATFORM_WINDOWS

extern daedalusCore::Application* daedalusCore::createApplication();

int main(int argc, char** argv)
{
	auto app = daedalusCore::createApplication();
	app->init();
	app->run();
	delete app;
}

#endif