#include "ddpch.h"
#include "vertexArray.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/buffers/openGLVertexArray.h"

namespace daedalusCore { namespace graphics { namespace buffers {

    VertexArray* VertexArray::Create()
    {
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::None:
			DD_CORE_ASSERT(false, "RendererAPI::None is not supported"); return nullptr;
		case RendererAPI::OpenGL:
			return new OpenGLVertexArray();
		}

		DD_CORE_ASSERT(false, "RendererAPI Unkown");
		return nullptr;
    }

} } }