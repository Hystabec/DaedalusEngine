#pragma once

#include <deque>
#include "renderer2D.h"

namespace daedalusCore { namespace graphics {

	class SimpleRenderer2D : public Renderer2D
	{
	private:
		std::deque<const Renderable2D*> m_renderQueue;
	public:
		void submit(const Renderable2D* renderable) override;
		void flush() override;
	};

} }