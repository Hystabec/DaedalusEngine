#pragma once
#include "graphics/renderer/renderingContext.h"

struct GLFWwindow;

namespace daedalusCore { namespace graphics {

	class OpenGlContext : public RenderingContext
	{
	private:
		GLFWwindow* m_windowHandle;

	public:
		OpenGlContext(GLFWwindow* windowHandle);

		void swapBuffers() override;
	};

} }