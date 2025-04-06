#pragma once

#include "maths/vec4.h"
#include "graphics/buffers/vertexArray.h"

namespace daedalusCore { namespace graphics {

	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0,
			OpenGL = 1
		};
	public:
		virtual void init() = 0;
		virtual void shutdown() {};

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void setClearColour(const maths::vec4& colour) = 0;
		virtual void clear() = 0;

		virtual void drawIndexedArray(const shr_ptr<graphics::buffers::VertexArray>& vertexArray) = 0;

		inline static API getAPI() { return s_API; }
	private:
		static API s_API;
	};

} }