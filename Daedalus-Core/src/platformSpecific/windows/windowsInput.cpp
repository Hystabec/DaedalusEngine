#include "ddpch.h"
#include "windowsInput.h"

#include <glfw3.h>
#include "applicationCore.h"

namespace daedalusCore { namespace application {

	Input* Input::m_instance = new windowsInput();

	bool windowsInput::GetKeyBase(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool windowsInput::GetKeyUpBase(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_RELEASE;
	}

	bool windowsInput::GetKeyDownBase(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS;
	}

	bool windowsInput::GetMouseButtonBase(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, button);
		return state == GLFW_PRESS;
	}

	std::pair<float, float> windowsInput::GetMousePositionBase()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float windowsInput::GetMouseXBase()
	{
		auto[x,y] = GetMousePositionBase();
		return x;
	}

	float windowsInput::GetMouseYBase()
	{
		auto[x,y] = GetMousePositionBase();
		return y;
	}

} }
