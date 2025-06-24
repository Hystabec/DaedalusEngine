#include <Daedalus.h>
#include "imgui.h"

#include "editorLayer.h"

#include "entryPoint.h"

class Editor : public daedalusCore::Application
{
public:
	Editor()
		: Application("Editor", 1280, 720, true)
	{
		this->pushLayer(new EditorLayer());
	}

	~Editor()
	{
	}
};

daedalusCore::Application* daedalusCore::createApplication()
{
	return new Editor();
}