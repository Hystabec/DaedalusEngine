#include <Daedalus.h>

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
	{
	}

	~SandBox()
	{
	}
};

daedalusCore::Application* daedalusCore::CreateApplication()
{
	return new SandBox();
}