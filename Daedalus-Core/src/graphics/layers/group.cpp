#include "group.h"

namespace daedalusCore { namespace graphics {

	Group::Group(const maths::mat4& transform)
		: m_transformationMatrix(transform)
	{
	}

	Group::~Group()
	{
		for (const Renderable2D* renderable : m_renderables)
			delete renderable;
	}

	void Group::add(Renderable2D* renderable)
	{
		m_renderables.push_back(renderable);
	}

	void Group::submit(Renderer2D* renderer) const
	{
		renderer->push(m_transformationMatrix);
		for (const Renderable2D* renderable : m_renderables)
			renderable->submit(renderer);
		renderer->pop();
	}

} }