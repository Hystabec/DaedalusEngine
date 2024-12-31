#include "ddpch.h"
#include "renderer.h"

namespace daedalusCore { namespace graphics {

	Renderer::sceneData* Renderer::m_sceneData = new Renderer::sceneData;
	
	void Renderer::begin(OrthographicCamera& othoCamera)
	{
		DD_CORE_ASSERT(!m_sceneData->inUse, "Renderer::end() not called");
		m_sceneData->projectionViewMatrix = othoCamera.getProjectViewMatrix();
		m_sceneData->inUse = true;
	}

	void Renderer::end()
	{
		DD_CORE_ASSERT(m_sceneData->inUse, "Renderer::begin() not called");
		m_sceneData->inUse = false;
	}

	void Renderer::submit(const std::shared_ptr<graphics::buffers::VertexArray>& vertexArray, const std::shared_ptr<graphics::Shader>& shader)
	{
		shader->enable();
		shader->setUniformMat4(m_sceneData->projectionViewMatrix, "u_projView");

		vertexArray->bind();
		RenderCommands::drawIndexed(vertexArray);
	}

} }