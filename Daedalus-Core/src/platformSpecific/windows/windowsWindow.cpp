#include "ddpch.h"
#include "windowsWindow.h"

#include <glfw3.h>

#include "events/windowEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

#include "platformSpecific/openGL/graphics/renderer/openGLContext.h"

namespace daedalusCore { namespace application {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		DD_CORE_LOG_ERROR("GLFW Error ({}): {}", error, description);
	}

	uni_ptr<Window> Window::Create(const WindowProperties& props)
	{
		return std::make_unique<WindowsWindow>(props);
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		m_data.Title = props.Title;
		m_data.Width = props.Width;
		m_data.Height = props.Height;
		m_data.Vsync = props.VSync;

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			DD_CORE_ASSERT(success, "GLFW failed to initialize");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}


		m_window = glfwCreateWindow((int)props.Width, (int)props.Height, m_data.Title.c_str(), nullptr, nullptr);

		if (!m_window)
			DD_CORE_ASSERT(false, "failed to create window");

		m_renderingContext = new graphics::OpenGlContext(m_window);

		glfwSetWindowUserPointer(m_window, &m_data);
		setVSync(props.VSync);

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				data.Width = width;
				data.Height = height;

				event::WindowResizedEvent event(width, height);
				data.EventCallBack(event);
			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				event::WindowClosedEvent event;
				data.EventCallBack(event);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					event::KeyPressedEvent event(key);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					event::KeyReleasedEvent event(key);
					data.EventCallBack(event);
					break;
				}
				case GLFW_REPEAT:
				{
					event::KeyHeldEvent event(key, 1);
					data.EventCallBack(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				event::KeyTypedEvent event(keycode);
				data.EventCallBack(event);
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					event::MouseButtonPressedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					event::MouseButtonReleasedEvent event(button);
					data.EventCallBack(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				event::MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallBack(event);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				event::MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallBack(event);
			});
	}

	WindowsWindow::~WindowsWindow()
	{
		shutdown();
	}

	void WindowsWindow::update()
	{
		glfwPollEvents();
		m_renderingContext->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_data.Vsync = enabled;
	}

	bool WindowsWindow::isVSync() const
	{
		return m_data.Vsync;
	}

	void WindowsWindow::init(const WindowProperties& props)
	{
		init(props);
	}

	void WindowsWindow::shutdown()
	{
		glfwDestroyWindow(m_window);
	}

} }