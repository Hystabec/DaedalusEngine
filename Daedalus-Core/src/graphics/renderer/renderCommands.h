#pragma once
#include "rendererAPI.h"

namespace daedalus { namespace graphics {

	class RenderCommands
	{
	public:
		inline static void init() { DD_PROFILE_FUNCTION(); s_rendererAPI->init(); }
		inline static void shutdown() { s_rendererAPI->shutdown(); }

		inline static void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_rendererAPI->setViewport(x, y, width, height);
		}

		inline static void setClearColour(const maths::Vec4& colour) { s_rendererAPI->setClearColour(colour); }
		inline static void clear() { s_rendererAPI->clear(); }

		inline static void drawIndexed(const Shr_ptr<graphics::buffers::VertexArray>& vertexArray, uint32_t indexCount = 0) { s_rendererAPI->drawIndexed(vertexArray, indexCount); }

	private:
		static RendererAPI* s_rendererAPI;
	};

} }