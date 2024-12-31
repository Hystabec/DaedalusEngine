#pragma once
#include "renderCommands.h"

namespace daedalusCore { namespace graphics {

	class Renderer
	{
	public:
		static void begin();
		static void end();

		static void submit(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray);

		static inline RendererAPI::API getCurrentAPI() { return RendererAPI::getAPI(); }
	};

} }