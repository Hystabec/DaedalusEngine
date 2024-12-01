#pragma once

#include "../renderer2D.h"
#include "../renderable2D.h"

namespace daedalusCore { namespace graphics{

	class Layer
	{
	protected:
		Renderer2D* m_renderer;
		std::vector<Renderable2D*> m_renderableObjects;
		Shader* m_shader;
		maths::mat4 m_projectionMatrix;
	
	protected:
		Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);
	public:
		virtual ~Layer();

		virtual void add(Renderable2D* renderable);
		virtual void render();
	};

}}