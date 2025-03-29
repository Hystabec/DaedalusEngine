#pragma once
#include "rendererAPI.h"

namespace daedalusCore { namespace graphics {

	class RenderCommands
	{
	public:
		inline static void setClearColour(const maths::vec4& colour) { s_rendererAPI->setClearColour(colour); }
		inline static void clear() { s_rendererAPI->clear(); }

		inline static void drawIndexed(const shr_ptr<graphics::buffers::VertexArray>& vertexArray) { s_rendererAPI->drawIndexedArray(vertexArray); }

	private:
		static RendererAPI* s_rendererAPI;
	};

} }