#pragma once
#include "graphics/renderer/rendererAPI.h"

namespace daedalusCore { namespace graphics {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void init() override;

		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void setClearColour(const maths::vec4& colour) override;
		void clear() override;

		void drawIndexedArray(const shr_ptr<graphics::buffers::VertexArray>& vertexArray) override;
	};

} }