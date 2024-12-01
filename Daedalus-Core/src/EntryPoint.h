#pragma once

#ifdef DD_PLATFORM_WINDOWS

extern daedalusCore::Application* daedalusCore::CreateApplication();

int main(int argc, char** argv)
{
	printf("Engine\n");
	auto app = daedalusCore::CreateApplication();
	app->Run();
	delete app;
}

#endif