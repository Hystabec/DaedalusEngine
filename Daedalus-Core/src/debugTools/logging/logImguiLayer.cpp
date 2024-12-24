#include "ddpch.h"
#include "logImguiLayer.h"
#include <imgui.h>

/*
This isnt a great implementation of an imgui log layer and should be either improved or removed
*/

namespace daedalusCore { namespace debug {

	logImguiLayer::logImguiLayer()
	{
	}

	void logImguiLayer::ImGuiRender()
	{
		static bool open = true;
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos()));
		ImGui::Begin("LogLayer", &open, ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground| ImGuiWindowFlags_AlwaysAutoResize);
		
		int i = 0;
		while(i < textToDisplay.size())
		{
			if (textToDisplay[i].second <= 0)
			{
				textToDisplay.erase(textToDisplay.begin() + i);
				continue;
			}

			ImGui::Text(textToDisplay[i].first.c_str());
			textToDisplay[i].second--;
			i++;
		}

		ImGui::End();
	}

	void logImguiLayer::SubmitText(std::string text)
	{
		textToDisplay.push_back({ text, 300 });
	}

} }