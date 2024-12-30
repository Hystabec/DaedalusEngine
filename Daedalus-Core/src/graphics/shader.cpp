#include "ddpch.h"
#include "shader.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/openGLShader.h"
#include "utils/fileUtils.h"

namespace daedalusCore { namespace graphics {

	Shader* graphics::Shader::create(const char* vertex, const char* frag, bool fromFile)
	{
		std::string verSrc = vertex;
		std::string fragSrc = frag;

		if (fromFile)
		{
			verSrc = utils::read_file(vertex);
			fragSrc = utils::read_file(frag);
		}

		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLShader(verSrc.c_str(), fragSrc.c_str());
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} }