#pragma once
#include "graphics/renderer/rendererAPI.h"

namespace daedalus { namespace graphics {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void init() override;

		void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		void setClearColour(const maths::Vec4& colour) override;
		void clear() override;

		void drawIndexed(const IntrusivePtr<graphics::buffers::VertexArray>& vertexArray, uint32_t indexCount = 0) override;
		void drawLines(const IntrusivePtr<graphics::buffers::VertexArray>& vertexArray, uint32_t vertexCount) override;

		void setLineThickness(float thickness) override;
	};

} }