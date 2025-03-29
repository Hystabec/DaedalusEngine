#include "ddpch.h"
#include "shader.h"

#include "graphics/renderer/renderer.h"
#include "utils/fileUtils.h"

#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/openGLShader.h"
#endif

namespace daedalusCore { namespace graphics {

	Shader* graphics::Shader::create(const std::string& vertex, const std::string& frag, bool fromFile)
	{
		std::string verSrc = vertex;
		std::string fragSrc = frag;

		if (fromFile)
		{
			verSrc = utils::read_file(vertex.c_str());
			fragSrc = utils::read_file(frag.c_str());
		}

		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return new OpenGLShader(verSrc.c_str(), fragSrc.c_str());
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} }