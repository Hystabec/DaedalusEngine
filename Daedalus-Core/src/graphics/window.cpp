#include "ddpch.h"
#include "window.h"

namespace daedalusCore { namespace graphics {

	void window_Resize(GLFWwindow* window, int width, int height)
	{
		glViewport(0, 0, width, height);
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_width = width;
		win->m_height = height;
	}

	void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* t_window = (Window*)glfwGetWindowUserPointer(window);

		t_window->m_keys[key] = action != GLFW_RELEASE;
	}

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Window* t_window = (Window*)glfwGetWindowUserPointer(window);

		t_window->m_MouseButtons[button] = action != GLFW_RELEASE;
	}

	void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Window* t_window = (Window*)glfwGetWindowUserPointer(window);

		t_window->m_mouseX = xpos;
		t_window->m_mouseY = ypos;
	}

	Window::Window(const char* title, int width, int height) : m_title(title), m_width(width), m_height(height)
	{
		if (!glfwInit())
		{
			std::cout << "Error - GLFW initialize failed" << std::endl;
			return;
		}

		m_window = glfwCreateWindow(width, height, title, NULL, NULL);

		if (!m_window)
		{
			glfwTerminate();
			std::cout << "Error - Failed to create GLFW window" << std::endl;
			return;
		}

		glfwMakeContextCurrent(m_window);
		glfwSetWindowUserPointer(m_window, this);
		glfwSetFramebufferSizeCallback(m_window, window_Resize);

		glfwSetKeyCallback(m_window, keyCallback);
		glfwSetMouseButtonCallback(m_window, mouseButtonCallback);
		glfwSetCursorPosCallback(m_window, cursorPositionCallback);

		glfwSwapInterval(0.0f);

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Error - GLEW initialize failed" << std::endl;
			return;
		}

		//std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_keys[i] = false;
			m_keysPrevious[i] = false;
		}

		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
			m_MouseButtonsPrevious[i] = false;
		}
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
		memcpy(m_keysPrevious, m_keys, sizeof(bool) * MAX_KEYS);
		memcpy(m_MouseButtonsPrevious, m_MouseButtons, sizeof(bool) * MAX_BUTTONS);

		//this takes a lot of time - might not need it or only check for errors after preforming certain actions? 
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
			std::cout << "openGL Error: " << error << std::endl;

		//this also takes a lot of time - might not be able to do anything about it
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

	bool Window::getKeyUp(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
		{
			std::cout << "Error - keycode out of range" << std::endl;
			return false;
		}

		if (!m_keys[keycode] && m_keysPrevious[keycode])
			return true;
		else
			return false;
	}

	bool Window::getKeyDown(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
		{
			std::cout << "Error - keycode out of range" << std::endl;
			return false;
		}

		if (m_keys[keycode] && !m_keysPrevious[keycode])
			return true;
		else
			return false;
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

	bool Window::getMouseUp(unsigned int buttonCode) const
	{
		if (buttonCode >= MAX_BUTTONS)
		{
			std::cout << "Error - buttonCode out of range" << std::endl;
			return false;
		}

		if (!m_MouseButtons[buttonCode] && m_MouseButtonsPrevious[buttonCode])
			return true;
		else
			return false;
	}

	bool Window::getMouseDown(unsigned int buttonCode) const
	{
		if (buttonCode >= MAX_BUTTONS)
		{
			std::cout << "Error - buttonCode out of range" << std::endl;
			return false;
		}

		if (m_MouseButtons[buttonCode] && !m_MouseButtonsPrevious[buttonCode])
			return true;
		else
			return false;
	}

	void Window::getMousePosition(double& xPos, double& yPos) const
	{
		xPos = m_mouseX;
		yPos = m_mouseY;
	}

} }