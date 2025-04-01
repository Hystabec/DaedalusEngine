#include "ddpch.h"
#include "vertexBuffer.h"

#include "graphics/renderer/renderer.h"
#ifdef DD_RENDER_USING_OPENGL
#include "platformSpecific/openGL/graphics/buffers/openGLVertexBuffer.h"
#endif

namespace daedalusCore { namespace graphics { namespace buffers {

	shr_ptr<VertexBuffer> buffers::VertexBuffer::create(float* verticies, uint32_t size)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
#ifdef DD_RENDER_USING_OPENGL
		case RendererAPI::API::OpenGL:
			return std::make_shared<OpenGlVertexBuffer>(verticies, size);
#endif
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} } }