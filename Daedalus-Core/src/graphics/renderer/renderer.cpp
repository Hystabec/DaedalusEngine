#include "ddpch.h"
#include "renderer.h"

namespace daedalusCore { namespace graphics {

	void Renderer::Begin()
	{
	}

	void Renderer::End()
	{
	}

	void Renderer::Submit(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray)
	{
		vertexArray->Bind();
		RenderCommands::DrawIndexed(vertexArray);
	}

} }