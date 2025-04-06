#include <Daedalus.h>
#include "imgui.h"

#include "generalLayer.h"
#include "2DTestLayer.h"

#include "entryPoint.h"

class SandBox : public daedalusCore::Application
{
public:
	SandBox()
		: Application("SandBox", 1280, 720, true)
	{
		//this->pushLayer(new generalLayer());
		this->pushLayer(new Layer2D());
	}

	~SandBox()
	{
	}
};

daedalusCore::Application* daedalusCore::createApplication()
{
	return new SandBox();
}