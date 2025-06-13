#include <Daedalus.h>
#include "imgui.h"

#include "entryPoint.h"

#include "Jumper/jumperLayer.h"

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
		: Application("Jumper", 1280, 720, true)
	{
		this->pushLayer(new jumper::JumperLayer());
	}

	~SandBox()
	{
	}
};

daedalusCore::Application* daedalusCore::createApplication()
{
	return new SandBox();
}