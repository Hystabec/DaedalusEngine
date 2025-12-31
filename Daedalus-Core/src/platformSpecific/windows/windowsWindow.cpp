#include "ddpch.h"
#include "windowsWindow.h"

#include "events/windowEvent.h"
#include "events/keyEvent.h"
#include "events/mouseEvent.h"

#include "application/input/inputCodeConversion.h"

#include "platformSpecific/openGL/graphics/renderer/openGLContext.h"

#include <glfw3.h>
#include <stb_image.h>

namespace daedalus { namespace application {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		DD_CORE_LOG_ERROR("GLFW Error ({}): {}", error, description);
	}

	ScopedPtr<Window> Window::Create(const WindowProperties& props)
	{
		//static_assert(std::is_convertible_v<WindowsWindow, Window>);
		return make_scoped_ptr<WindowsWindow>(props);;
	}

	WindowsWindow::WindowsWindow(const WindowProperties& props)
	{
		DD_PROFILE_FUNCTION();
		m_data.Title = props.Title;
		m_data.Width = props.Width;
		m_data.Height = props.Height;
		m_data.Vsync = props.VSync;

		if (!s_GLFWInitialized)
		{
			DD_PROFILE_SCOPE("glfwInit");
			int success = glfwInit();
			DD_CORE_ASSERT(success, "GLFW failed to initialize");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		{
			DD_PROFILE_SCOPE("glfwCreateWindow");
			m_window = glfwCreateWindow((int)props.Width, (int)props.Height, m_data.Title.c_str(), nullptr, nullptr);

			if (!m_window)
				DD_CORE_ASSERT(false, "failed to create window");
		}

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
					event::KeyPressedEvent event(application::utils::glfw_keycode_to_DD_keycode(key));
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					event::KeyReleasedEvent event(application::utils::glfw_keycode_to_DD_keycode(key));
					data.EventCallBack(event);
					break;
				}
				case GLFW_REPEAT:
				{
					event::KeyHeldEvent event(application::utils::glfw_keycode_to_DD_keycode(key), 1);
					data.EventCallBack(event);
					break;
				}
				}
			});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				event::KeyTypedEvent event(application::utils::glfw_keycode_to_DD_keycode(keycode));
				data.EventCallBack(event);
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					event::MouseButtonPressedEvent event(application::utils::glfw_keycode_to_DD_keycode(button));
					data.EventCallBack(event);
					break;
				}
				case GLFW_RELEASE:
				{
					event::MouseButtonReleasedEvent event(application::utils::glfw_keycode_to_DD_keycode(button));
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

		glfwSetDropCallback(m_window, [](GLFWwindow* window, int pathCount, const char* paths[])
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				std::vector<std::filesystem::path> filePaths(pathCount);
				for (int i = 0; i < pathCount; i++)
					filePaths[i] = paths[i];

				event::WindowDropEvent event(std::move(filePaths));
				data.EventCallBack(event);
			});
	}

	WindowsWindow::~WindowsWindow()
	{
		DD_PROFILE_FUNCTION();
		shutdown();
	}

	void WindowsWindow::update()
	{
		DD_PROFILE_FUNCTION();
		glfwPollEvents();
		m_renderingContext->swapBuffers();
	}

	void WindowsWindow::setVSync(bool enabled)
	{
		DD_PROFILE_FUNCTION();

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

	void WindowsWindow::setWindowName(const std::string& name)
	{
		glfwSetWindowTitle(m_window, name.c_str());
	}

	void WindowsWindow::setWindowIcon(const std::filesystem::path& path)
	{
		GLFWimage image;
		image.pixels = stbi_load(path.string().c_str(), &image.width, &image.height, 0, 4);
		glfwSetWindowIcon(m_window, 1, &image);
		stbi_image_free(image.pixels);
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