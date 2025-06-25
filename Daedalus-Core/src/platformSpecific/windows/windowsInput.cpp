#include "ddpch.h"
#include "application/input/input.h"

#include <glfw3.h>
#include "application/applicationCore.h"

namespace daedalus::application {

	/*bool Input::getKey(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetKey(window, keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}*/

	bool Input::getKeyUp(InputCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_RELEASE;
	}

	bool Input::getKeyDown(InputCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetKey(window, (int)keycode);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::getMouseButton(InputCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		auto state = glfwGetMouseButton(window, (int)button);
		return state == GLFW_PRESS;
	}

	maths::vec2 Input::getMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::get().getWindow()->getNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return { (float)x, (float)y };
	}

	float Input::getMouseX()
	{
		auto[x,y] = getMousePosition();
		return x;
	}

	float Input::getMouseY()
	{
		auto[x,y] = getMousePosition();
		return y;
	}

}