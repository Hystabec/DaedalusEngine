#include "ddpch.h"
#include "vertexBuffer.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/buffers/openGLVertexBuffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	VertexBuffer* buffers::VertexBuffer::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGlVertexBuffer(verticies, size);
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} } }