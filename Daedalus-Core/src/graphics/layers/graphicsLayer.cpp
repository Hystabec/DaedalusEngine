#include "ddpch.h"
#include "graphicsLayer.h"

#include "graphics/renderer2D.h"
#include "graphics/renderable2D.h"
#include "graphics/shader.h"

namespace daedalusCore { namespace graphics {

	Layer::~Layer()
	{
		delete m_shader;
		delete m_renderer;

		for (int i = 0; i < m_renderableObjects.size(); i++)
			delete m_renderableObjects[i];
	}

	void Layer::add(Renderable2D* renderable)
	{
		m_renderableObjects.push_back(renderable);
	}

	void Layer::render()
	{
		m_shader->enable();
		m_renderer->begin();

		for (const Renderable2D* renerable : m_renderableObjects)
			renerable->submit(m_renderer);

		m_renderer->end();
		m_renderer->render();
		m_shader->disable();
	}

	Layer::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix)
		: m_renderer(renderer), m_shader(shader), m_projectionMatrix(projectionMatrix)
	{
		m_shader->enable();
		m_shader->setUniformMat4(m_projectionMatrix, "pr_matrix");
		m_shader->disable();
	}

} }
