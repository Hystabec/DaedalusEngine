#include <Daedalus.h>
#include "imgui.h"

#include "generalLayer.h"
#include "2DTestLayer.h"
#include "alphaBlendingTestLayer.h"
#include "dockspaceTesting.h"

#include "entryPoint.h"

class SandBox : public daedalus::Application
{
public:
	SandBox()
		: Application("SandBox", 1280, 720, true)
	{
		//this->pushLayer(new generalLayer());
		this->pushLayer(new Layer2D());
		//this->pushLayer(new AlphaBlendingTestLayer());
		//this->pushLayer(new DockspaceTesting());
	}

	~SandBox()
	{
	}
};

daedalus::Application* daedalus::createApplication()
{
	return new SandBox();
}