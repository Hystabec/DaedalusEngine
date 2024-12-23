#include "ddpch.h"
#include "logImguiLayer.h"
#include <imgui.h>

namespace daedalusCore { namespace debug {

	logImguiLayer::logImguiLayer()
	{
	}

	void logImguiLayer::ImGuiRender()
	{
		ImGui::Begin("LogLayer");
		ImGui::Text("Log layer");
		ImGui::End();
	}

} }