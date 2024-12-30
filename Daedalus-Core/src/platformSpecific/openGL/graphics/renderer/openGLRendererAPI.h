#pragma once
#include "graphics/renderer/rendererAPI.h"

namespace daedalusCore { namespace graphics {

	class OpenGLRendererAPI : public RendererAPI
	{
	public:
		void SetClearColour(const maths::vec4& colour) override;
		void Clear() override;

		void DrawIndexedArray(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray) override;
	};

} }