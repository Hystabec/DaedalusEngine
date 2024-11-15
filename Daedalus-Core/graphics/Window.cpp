#include "Window.h"

namespace daedalusCore { namespace graphics {

	void window_Resize(GLFWwindow* window, int width, int height);

	Window::Window(const char* title, int width, int height) : m_title(title), m_width(width), m_height(height)
	{
		if (!glfwInit())
			std::cout << "Error - GLFW initialize failed" << std::endl;

		m_window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (!m_window)
		{
			glfwTerminate();
			std::cout << "Error - Failed to create GLFW window" << std::endl;
			return;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetWindowSizeCallback(m_window, window_Resize);

		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
		glfwSetCursorPosCallback(m_window, cursorPositionCallback);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error - Failed to initialize GLEW" << std::endl;
			return;
		}

		std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

		for (int i = 0; i < MAX_KEYS; i++)
			m_keys[i] = false;

		for (int i = 0; i < MAX_BUTTONS; i++)
			m_MouseButtons[i] = false;
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

	bool Window::getKey(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
		{
			std::cout << "Error - keycode out of range" << std::endl;
			return false;
		}

		return m_keys[keycode];
	}

	bool Window::getMouse(unsigned int buttonCode) const
	{
		if (buttonCode >= MAX_BUTTONS)
		{
			std::cout << "Error - buttonCode out of range" << std::endl;
			return false;
		}

		return m_MouseButtons[buttonCode];
	}

	void Window::getMousePosition(double& xPos, double& yPos) const
	{
		xPos = m_mouseX;
		yPos = m_mouseY;
	}

	void window_Resize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
	}

	void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* t_window = (Window*)glfwGetWindowUserPointer(window);
		
		t_window->m_keys[key] = action != GLFW_RELEASE;
	}

	void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* t_window = (Window*)glfwGetWindowUserPointer(window);

		t_window->m_MouseButtons[button] = action != GLFW_RELEASE;
	}

	void Window::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Window* t_window = (Window*)glfwGetWindowUserPointer(window);

		t_window->m_mouseX = xpos;
		t_window->m_mouseY = ypos;
	}

} }