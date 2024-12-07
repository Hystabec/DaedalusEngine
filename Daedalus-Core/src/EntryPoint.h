#pragma once

#ifdef DD_PLATFORM_WINDOWS

extern daedalusCore::Application* daedalusCore::CreateApplication();

int main(int argc, char** argv)
{
	auto app = daedalusCore::CreateApplication();
	app->Init();
	app->Run();
	delete app;
}

#endif