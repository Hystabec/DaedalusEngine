#include "ddpch.h"
#include "windowsInput.h"

#include <glfw3.h>
#include "application/applicationCore.h"

namespace daedalusCore { namespace application {

	Input* Input::s_instance = new windowsInput();

	/*bool windowsInput::GetKeyBase(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}*/

	bool windowsInput::getKeyUpBase(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_RELEASE;
	}

	bool windowsInput::getKeyDownBase(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool windowsInput::getMouseButtonBase(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetMouseButton(window, button);
		return state == GLFW_PRESS;
	}

	maths::vec2 windowsInput::getMousePositionBase()
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float windowsInput::getMouseXBase()
	{
		auto[x,y] = getMousePositionBase();
		return x;
	}

	float windowsInput::getMouseYBase()
	{
		auto[x,y] = getMousePositionBase();
		return y;
	}

} }
