#include "ddpch.h"
#include "imGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>

#include <glfw3.h>
#include "applicationCore.h"

namespace daedalusCore { namespace application {

	ImGuiLayer::ImGuiLayer()
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::Attach()
	{
		/*ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;*/

		//ImGui_ImplOpenGL3_Init("#version 330 core");
	}

	void ImGuiLayer::Detach()
	{
	}

	void ImGuiLayer::Update()
	{
		/*ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		ImGuiIO& io = ImGui::GetIO();
		Window* win = Application::Get().GetWindow();
		io.DisplaySize = ImVec2(win->GetWidth(), win->GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.f);
		m_time = time;

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());*/
	}

	void ImGuiLayer::OnEvent(event::Event& event)
	{
	}

} }