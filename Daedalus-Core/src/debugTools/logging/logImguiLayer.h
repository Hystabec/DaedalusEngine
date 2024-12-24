#pragma once

#include "application/layers/layer.h"

/*
This isnt a great implementation of an imgui log layer and should be either improved or removed
*/

namespace daedalusCore { namespace debug {

	class logImguiLayer : public application::Layer
	{
	private:
		std::vector<std::pair<std::string, unsigned int>> textToDisplay;
	public:
		logImguiLayer();

		void ImGuiRender();

		void SubmitText(std::string text);
	};

} }