#include <Daedalus.h>

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
	{
		m_applicationName = "Sandbox Application";
	}

	~SandBox()
	{
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}