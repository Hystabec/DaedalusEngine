#pragma once
#include "graphics/renderer/rendererAPI.h"

namespace daedalusCore { namespace graphics {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void setClearColour(const maths::vec4& colour) override;
		void clear() override;

		void drawIndexedArray(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray) override;
	};

} }