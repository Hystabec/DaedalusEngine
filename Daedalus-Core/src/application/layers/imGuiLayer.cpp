#include "ddpch.h"
#include "imGuiLayer.h"

#include "imgui.h"
#include "platformSpecific/openGL/ImGuiOpenglRenderer.h"

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
		
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		

		ImGui_ImplOpenGL3_Init("#version 410 core");
	}

	void ImGuiLayer::Detach()
	{
	}

	void ImGuiLayer::Update()
	{

		ImGuiIO& io = ImGui::GetIO();
		Window* win = Application::Get().GetWindow();
		io.DisplaySize = ImVec2(win->GetWidth(), win->GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_time > 0.0f ? (time - m_time) : (1.0f / 60.f);
		m_time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(event::Event& event)
	{
	}

} }