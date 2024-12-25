#include "ddpch.h"
#include "indexBuffer.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/buffers/openGLIndexBuffer.h"

namespace daedalusCore { namespace graphics { namespace buffers {

	IndexBuffer* buffers::IndexBuffer::Create(uint32_t* indices, uint32_t size)
	{
		switch (Renderer::GetCurrentAPI())
		{
		case RendererAPI::None: 
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL: 
			return new OpenGLIndexBuffer(indices, size);
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
	}

} } }