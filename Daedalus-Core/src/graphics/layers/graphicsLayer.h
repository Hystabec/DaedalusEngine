#pragma once

#include "maths/mat4.h"

namespace daedalusCore { namespace graphics{

	class Renderer2D;
	class Renderable2D;
	class Shader;
		 
	class Layer
	{
	public:
		virtual ~Layer();

		virtual void add(Renderable2D* renderable);
		virtual void render();

		inline const std::vector<Renderable2D*>& getRenderables() const { return m_renderableObjects; }

	protected:
		Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);

	protected:
		Renderer2D* m_renderer;
		std::vector<Renderable2D*> m_renderableObjects;
		Shader* m_shader;
		maths::mat4 m_projectionMatrix;
	};

}}