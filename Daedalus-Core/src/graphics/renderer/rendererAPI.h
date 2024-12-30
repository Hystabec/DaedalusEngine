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
		virtual void SetClearColour(const maths::vec4& colour) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexedArray(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray) = 0;

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

} }