#include "ddpch.h"
#include "texture.h"

#include "../renderer/renderer.h"
#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/rendering/openGLTexture.h"
#endif

namespace daedalusCore { namespace graphics {

	shr_ptr<Texture2D> Texture2D::create(const std::string& filePath)
    {
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGLTexture2D>(filePath);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
    }

} }