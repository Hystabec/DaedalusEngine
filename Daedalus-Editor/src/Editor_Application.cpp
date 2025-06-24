#include <Daedalus.h>
#include "imgui.h"

#include "editorLayer.h"

#include "entryPoint.h"

namespace daedalus
{

	class Editor : public Application
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

	Application* createApplication()
	{
		return new Editor();
	}

}