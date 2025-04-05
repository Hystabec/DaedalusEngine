#include "ddpch.h"
#include "renderer.h"

namespace daedalusCore { namespace graphics {

	Renderer::sceneData* Renderer::m_sceneData = new Renderer::sceneData;
	
	void Renderer::init()
	{
		RenderCommands::init();
	}

	void Renderer::onWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommands::setViewport(0, 0, width, height);
	}

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

	void Renderer::submit(const shr_ptr<graphics::buffers::VertexArray>& vertexArray, const shr_ptr<graphics::Shader>& shader, const maths::mat4& transform)
	{
		shader->enable();
		shader->setUniformMat4(m_sceneData->projectionViewMatrix, "u_projView");
		shader->setUniformMat4(transform, "u_transform");

		vertexArray->bind();
		RenderCommands::drawIndexed(vertexArray);
		//shader->disable();
	}

} }