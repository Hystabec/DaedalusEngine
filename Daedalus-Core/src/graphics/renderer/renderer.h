#pragma once
#include "renderCommands.h"

namespace daedalusCore { namespace graphics {

	class Renderer
	{
	public:
		static void Begin();
		static void End();

		static void Submit(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray);

		static inline RendererAPI::API GetCurrentAPI() { return RendererAPI::GetAPI(); }
	};

} }