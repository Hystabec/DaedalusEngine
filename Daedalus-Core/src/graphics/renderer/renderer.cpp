#include "ddpch.h"
#include "renderer.h"

#include "renderer2D.h"

namespace daedalus { namespace graphics {

	Renderer::sceneData* Renderer::m_sceneData = new Renderer::sceneData;
	
	void Renderer::init()
	{
		DD_PROFILE_FUNCTION();
		RenderCommands::init();
		Renderer2D::init();
	}

	void Renderer::shutdown()
	{
		RenderCommands::shutdown();
		Renderer2D::shutdown();
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

	void Renderer::submit(const Shr_ptr<graphics::buffers::VertexArray>& vertexArray, const Shr_ptr<graphics::Shader>& shader, const maths::Mat4& transform)
	{
		shader->bind();
		shader->setUniformMat4(m_sceneData->projectionViewMatrix, "u_projView");
		shader->setUniformMat4(transform, "u_transform");

		vertexArray->bind();
		RenderCommands::drawIndexed(vertexArray);
		//shader->disable();
	}

} }