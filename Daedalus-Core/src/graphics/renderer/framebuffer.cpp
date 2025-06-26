#include "ddpch.h"
#include "framebuffer.h"

#include "graphics/renderer/renderer.h"
#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/renderer/openGLFramebuffer.h"
#endif

namespace daedalus::graphics {

    Shr_ptr<Framebuffer> Framebuffer::create(const FramebufferSpecification& specification)
    {
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return create_shr_ptr<OpenGLFramebuffer>(specification);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
    }

}