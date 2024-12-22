#include "ddpch.h"
#include "imGuiLayer.h"

#include "imgui.h"
#include "platformSpecific/openGL/ImGuiOpenglRenderer.h"

#include "applicationCore.h"
#include "application/inputKeyCodes.h"

//temp
#include <glfw3.h>

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

		io.KeyMap[ImGuiKey_Tab] = INPUT_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = INPUT_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = INPUT_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = INPUT_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = INPUT_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = INPUT_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = INPUT_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = INPUT_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = INPUT_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = INPUT_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = INPUT_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = INPUT_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = INPUT_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = INPUT_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = INPUT_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_KeyPadEnter] = INPUT_KEY_KP_ENTER;
		io.KeyMap[ImGuiKey_A] = INPUT_KEY_A;
		io.KeyMap[ImGuiKey_C] = INPUT_KEY_C;
		io.KeyMap[ImGuiKey_V] = INPUT_KEY_V;
		io.KeyMap[ImGuiKey_X] = INPUT_KEY_X;
		io.KeyMap[ImGuiKey_Y] = INPUT_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = INPUT_KEY_Z;

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
		event::EventDispatcher dispatch(event);
		dispatch.Dispatch<event::MouseButtonPressedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatch.Dispatch<event::MouseButtonReleasedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatch.Dispatch<event::MouseMovedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnMouseMoveEvent));
		dispatch.Dispatch<event::MouseScrolledEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnMouseMouseScrolledEvent));
		dispatch.Dispatch<event::KeyPressedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnKeyPressedEvent));
		dispatch.Dispatch<event::KeyReleasedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnKeyReleasedEvent));
		dispatch.Dispatch<event::KeyTypedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnKeyTypedEvent));
		dispatch.Dispatch<event::WindowResizedEvent>(DD_BIND_EVENT_FUN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(event::MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButtonCode()] = true;
		return false;
	}

	bool ImGuiLayer::OnMouseButtonReleasedEvent(event::MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetButtonCode()] = false;
		return false;
	}

	bool ImGuiLayer::OnMouseMoveEvent(event::MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());
		return false;
	}

	bool ImGuiLayer::OnMouseMouseScrolledEvent(event::MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetX();
		io.MouseWheel += e.GetY();
		return false;
	}

	bool ImGuiLayer::OnKeyPressedEvent(event::KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;
		io.KeyCtrl = io.KeysDown[INPUT_KEY_LEFT_CONTROL] || io.KeysDown[INPUT_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[INPUT_KEY_LEFT_SHIFT] || io.KeysDown[INPUT_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[INPUT_KEY_LEFT_ALT] || io.KeysDown[INPUT_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[INPUT_KEY_LEFT_SUPER] || io.KeysDown[INPUT_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyReleasedEvent(event::KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetKeyCode()] = false;
		io.KeyCtrl = io.KeysDown[INPUT_KEY_LEFT_CONTROL] || io.KeysDown[INPUT_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[INPUT_KEY_LEFT_SHIFT] || io.KeysDown[INPUT_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[INPUT_KEY_LEFT_ALT] || io.KeysDown[INPUT_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[INPUT_KEY_LEFT_SUPER] || io.KeysDown[INPUT_KEY_RIGHT_SUPER];
		return false;
	}

	bool ImGuiLayer::OnKeyTypedEvent(event::KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keycode = e.GetKeyCode();
		if (keycode > 0 && keycode < 0x100000)
			io.AddInputCharacter((unsigned short)keycode);

		return false;
	}

	bool ImGuiLayer::OnWindowResizeEvent(event::WindowResizedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}

} }