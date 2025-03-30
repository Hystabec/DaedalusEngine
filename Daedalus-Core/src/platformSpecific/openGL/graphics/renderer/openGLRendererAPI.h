#pragma once
#include "graphics/renderer/rendererAPI.h"

namespace daedalusCore { namespace graphics {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void init() override;

		void setClearColour(const maths::vec4& colour) override;
		void clear() override;

		void drawIndexedArray(const shr_ptr<graphics::buffers::VertexArray>& vertexArray) override;
	};

} }