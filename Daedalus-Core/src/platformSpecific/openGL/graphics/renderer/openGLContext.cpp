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

#ifdef DD_USING_ASSERTS
		int versionMajor;
		int versionMinor;
		glGetIntegerv(GL_MAJOR_VERSION, &versionMajor);
		glGetIntegerv(GL_MINOR_VERSION, &versionMinor);

		DD_CORE_ASSERT(versionMajor > 4 || (versionMajor == 4 && versionMinor >= 5), "OpenGL version 4.5 or greater required");
#endif // DD_USING_ASSERTS

	}

	void OpenGlContext::swapBuffers()
	{
		glfwSwapBuffers(m_windowHandle);
	}

} }