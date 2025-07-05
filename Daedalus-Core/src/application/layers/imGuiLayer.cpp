#include "ddpch.h"
#include "imGuiLayer.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include "application/applicationCore.h"
#include "application/input/inputCodes.h"

//temp
#include <glfw3.h>


namespace daedalus { namespace application {

	ImGuiLayer::ImGuiLayer()
	{
	}

	ImGuiLayer::~ImGuiLayer()
	{
	}

	void ImGuiLayer::attach()
	{
		DD_PROFILE_FUNCTION();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
		//io.ConfigViewportsNoAutoMerge = true;
		//io.ConfigViewportsNoTaskBarIcon = true;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// Sets custom ImGui styling
		io.Fonts->AddFontFromFileTTF("resources/fonts/Noto_Sans/NotoSans-Bold.ttf", 18.0f);
		io.FontDefault = io.Fonts->AddFontFromFileTTF("resources/fonts/Noto_Sans/NotoSans-Medium.ttf", 18.0f);
		setDarkThemeColours();

		GLFWwindow* wind = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL(wind, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::detach()
	{
		DD_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::imGuiRender()
	{
	}

	void ImGuiLayer::onEvent(event::Event& event)
	{
		if (!m_allowEvents)
		{
			ImGuiIO& io = ImGui::GetIO();

			event.setHandled((event.isInCategory(event::EventCategory::Mouse) && io.WantCaptureMouse) || (event.isInCategory(event::EventCategory::Keyboard) && io.WantCaptureMouse));
		}
	}

	void ImGuiLayer::begin()
	{
		DD_PROFILE_FUNCTION();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiLayer::end()
	{
		DD_PROFILE_FUNCTION();

		ImGuiIO& io = ImGui::GetIO();
		Window* win = Application::get().getWindow();
		io.DisplaySize = ImVec2((float)win->getWidth(), (float)win->getHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::setDarkThemeColours()
	{
		auto& colours = ImGui::GetStyle().Colors;
		ImVec4 pink = { 1.0f, 0.0f, 1.0f, 1.0f };

		colours[ImGuiCol_WindowBg] =			ImVec4{ 0.10f, 0.10f, 0.10f, 1.0f };
		
		colours[ImGuiCol_Header] =				ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colours[ImGuiCol_HeaderHovered] =		ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f };
		colours[ImGuiCol_HeaderActive] =		ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colours[ImGuiCol_Button] =				ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colours[ImGuiCol_ButtonHovered] =		ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f };
		colours[ImGuiCol_ButtonActive] =		ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colours[ImGuiCol_FrameBg] =				ImVec4{ 0.25f, 0.25f, 0.25f, 1.0f };
		colours[ImGuiCol_FrameBgHovered] =		ImVec4{ 0.35f, 0.35f, 0.35f, 1.0f };
		colours[ImGuiCol_FrameBgActive] =		ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colours[ImGuiCol_Tab] =					ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colours[ImGuiCol_TabHovered] =			ImVec4{ 0.86f, 0.10f, 0.20f, 1.0f };
		colours[ImGuiCol_TabActive] =			ImVec4{ 0.76f, 0.00f, 0.10f, 1.0f };
		colours[ImGuiCol_TabUnfocused] =		ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colours[ImGuiCol_TabUnfocusedActive] =	ImVec4{ 0.20f, 0.20f, 0.20f, 1.0f };

		colours[ImGuiCol_TabSelectedOverline] = ImVec4{ 0.76f, 0.00f, 0.10f, 1.0f };

		colours[ImGuiCol_TitleBg] =				ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colours[ImGuiCol_TitleBgActive] =		ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };
		colours[ImGuiCol_TitleBgCollapsed] =	ImVec4{ 0.15f, 0.15f, 0.15f, 1.0f };

		colours[ImGuiCol_CheckMark] =			ImVec4{ 0.10f, 0.80f, 0.10f, 1.0f };
	}

} }