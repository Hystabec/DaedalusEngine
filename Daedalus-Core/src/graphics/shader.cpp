#include "ddpch.h"
#include "shader.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/openGLShader.h"

namespace daedalusCore { namespace graphics {

	Shader* graphics::Shader::create(const char* vertexPath, const char* fragPath)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLShader(vertexPath, fragPath);
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} }