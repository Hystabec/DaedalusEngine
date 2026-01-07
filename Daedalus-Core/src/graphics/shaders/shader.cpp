#include "ddpch.h"
#include "shader.h"

#include "graphics/renderer/renderer.h"
#include "utils/fileUtils.h"

#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/shaders/openGLShader.h"
#endif

namespace daedalus { namespace graphics {

	IntrusivePtr<Shader> Shader::create(const std::filesystem::path& filePath)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return make_intrusive_ptr<OpenGLShader>(filePath);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

	IntrusivePtr<Shader> graphics::Shader::create(const std::string& name, const std::string& vertex, const std::string& frag)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return make_intrusive_ptr<OpenGLShader>(name, vertex, frag);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} }