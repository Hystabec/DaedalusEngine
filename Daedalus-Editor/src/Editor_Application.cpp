#include "editorpch.h"

#include "entryPoint.h"
#include "editorLayer.h"

namespace daedalus
{

	class Editor : public Application
	{
	public:
		Editor(const ApplicationSpecification& spec)
			: Application(spec)
		{
			this->pushLayer(new editor::EditorLayer());
		}

		~Editor()
		{
		}
	};

	Application* createApplication(ApplicationCommandLineArgs args)
	{
		ApplicationSpecification spec;
		spec.name = "Daedalus Editor";
		spec.width = 1600;
		spec.height = 900;
		spec.vsync = true;
		spec.workingDirectory = "";
		spec.commandLineArgs = args;

		return new Editor(spec);
	}

}