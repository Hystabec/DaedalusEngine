#include "Window.h"

namespace daedalusCore { namespace graphics {

	void windowResize(GLFWwindow* window, int width, int height);

	Window::Window(const char* title, int width, int height) : m_title(title), m_width(width), m_height(height)
	{
		if (!glfwInit())
			std::cout << "Error - GLFW init failed" << std::endl;

		m_window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (!m_window)
		{
			glfwTerminate();
			std::cout << "Error - Failed to create GLFW window" << std::endl;
			return;
		}

		glfwMakeContextCurrent(m_window);

		glfwSetWindowSizeCallback(m_window, windowResize);
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	void Window::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::update()
	{
		glfwPollEvents();

		glfwSwapBuffers(m_window);
	}

	bool Window::closed() const
	{
		return glfwWindowShouldClose(m_window);
	}

	void windowResize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

} }