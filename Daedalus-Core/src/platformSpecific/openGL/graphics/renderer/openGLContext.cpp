#include "ddpch.h"
#include "openGLContext.h"

#include <GL/glew.h>
#include <glfw3.h>

namespace daedalusCore { namespace graphics {

	static bool s_GLEWInitialized = false;

	OpenGlContext::OpenGlContext(GLFWwindow* windowHandle)
		: m_windowHandle(windowHandle)
	{
		DD_CORE_ASSERT(windowHandle, "Window handle is nullptr");

		glfwMakeContextCurrent(m_windowHandle);

		if (!s_GLEWInitialized)
		{
			if (glewInit() != GLEW_OK)
				DD_CORE_ASSERT(false, "GLEW failed to initialize");
		}
	}

	void OpenGlContext::SwapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

} }