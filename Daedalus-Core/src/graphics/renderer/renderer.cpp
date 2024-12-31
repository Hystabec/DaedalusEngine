#include "ddpch.h"
#include "renderer.h"

namespace daedalusCore { namespace graphics {

	void Renderer::begin()
	{
	}

	void Renderer::end()
	{
	}

	void Renderer::submit(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray)
	{
		vertexArray->bind();
		RenderCommands::drawIndexed(vertexArray);
	}

} }