#include "editorpch.h"

#include "entryPoint.h"
#include "editorLayer.h"

namespace daedalus
{

	class Editor : public Application
	{
	public:
		Editor()
			: Application("Daedalus Editor", 1600, 900, true)
		{
			this->pushLayer(new editor::EditorLayer());
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