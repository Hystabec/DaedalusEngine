#pragma once

#include "core.h"

namespace daedalusCore {

	class DD_API Application
	{
	public:
		Application();
		~Application();

		int Run();
	};

	//To be defined in client
	Application* CreateApplication();

}