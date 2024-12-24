#include "ddpch.h"
#include "logImguiLayer.h"
#include <imgui.h>

namespace daedalusCore { namespace debug {

	logImguiLayer::logImguiLayer()
	{
	}

	void logImguiLayer::ImGuiRender()
	{
		static bool open = true;
		ImGui::Begin("LogLayer", &open, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground);
		ImGui::Text("Log layer");
		ImGui::End();
	}

} }