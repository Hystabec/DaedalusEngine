#pragma once
#include "rendererAPI.h"

namespace daedalusCore { namespace graphics {

	class RenderCommands
	{
	private:
		static RendererAPI* s_rendererAPI;

	public:
		inline static void SetClearColour(const maths::vec4& colour) { s_rendererAPI->SetClearColour(colour); }
		inline static void Clear() { s_rendererAPI->Clear(); }

		inline static void DrawIndexed(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray) { s_rendererAPI->DrawIndexedArray(vertexArray); }
	};

} }