#include "Daedalus.h"

#include "generalLayer.h"
#include "2DTestLayer.h"
#include "alphaBlendingTestLayer.h"
#include "dockspaceTesting.h"

#include "entryPoint.h"

//#include <imgui.h>

class SandBox : public daedalus::Application
{
public:
	SandBox(const daedalus::ApplicationSpecification& spec)
		: Application(spec)
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

daedalus::Application* daedalus::createApplication(daedalus::ApplicationCommandLineArgs args)
{
	daedalus::ApplicationSpecification spec;
	spec.name = "SandBox";
	spec.width = 1280;
	spec.height = 720;
	spec.vsync = true;
	spec.workingDirectory = "";
	spec.commandLineArgs = args;

	return new SandBox(spec);
}