#include "ddpch.h"
#include "indexBuffer.h"

#include "graphics/renderer/renderer.h"
#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/buffers/openGLIndexBuffer.h"
#endif

namespace daedalus { namespace graphics { namespace buffers {

	Shr_ptr<IndexBuffer> buffers::IndexBuffer::create(uint32_t* indices, uint32_t count)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return create_shr_ptr<OpenGLIndexBuffer>(indices, count);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} } }