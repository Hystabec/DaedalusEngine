#include "ddpch.h"
#include "uniformBuffer.h"

#include "graphics/renderer/renderer.h"
#include "platformSpecific/openGL/graphics/buffers/openGLUniformBuffer.h"

namespace daedalus::graphics::buffers {

	IntrusivePtr<UniformBuffer> UniformBuffer::create(uint32_t size, uint32_t binding)
	{
		switch (Renderer::getCurrentAPI())
		{
		case RendererAPI::API::None: DD_CORE_ASSERT(false, "RendererAPI::None is not currently suported"); return nullptr;
		case RendererAPI::API::OpenGL: return make_intrusive_ptr<OpenGlUniformBuffer>(size, binding);
		}

		DD_CORE_ASSERT(false, "Unknown RendererAPI");
		return nullptr;
	}

}