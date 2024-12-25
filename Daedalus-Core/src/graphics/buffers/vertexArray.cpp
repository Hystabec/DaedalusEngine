#include "ddpch.h"
#include "vertexArray.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/buffers/openGLVertexArray.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	VertexArray* buffers::VertexArray::Create(float* verticies, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGlVertexArray(verticies, size);
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} } }