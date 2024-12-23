#pragma once

#include "application/layers/layer.h"

namespace daedalusCore { namespace debug {

	class logImguiLayer : public application::Layer
	{
	public:
		logImguiLayer();

		void ImGuiRender();
	};

} }