#include "ddpch.h"
#include "texture.h"

#include "../renderer/renderer.h"
#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/rendering/openGLTexture.h"
#endif

namespace daedalus::graphics {

	Shr_ptr<Texture2D> Texture2D::create(const TextureSpecification& specification)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return create_shr_ptr<OpenGLTexture2D>(specification);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

	Shr_ptr<Texture2D> Texture2D::create(const std::filesystem::path& filePath)
    {
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return create_shr_ptr<OpenGLTexture2D>(filePath);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
    }

}