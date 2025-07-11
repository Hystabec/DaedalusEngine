#pragma once
#include "graphics/renderer/renderingContext.h"

struct GLFWwindow;

namespace daedalus { namespace graphics {

	class OpenGlContext : public RenderingContext
	{
	public:
		OpenGlContext(GLFWwindow* windowHandle);

		void swapBuffers() override;

	private:
		GLFWwindow* m_windowHandle;
	};

} }