#pragma once

#include <deque>
#include "renderer2D.h"
#include "staticSprite.h"

namespace daedalusCore { namespace graphics {

	class SimpleRenderer2D : public Renderer2D
	{
	private:
		std::deque<const StaticSprite*> m_renderQueue;
	public:
		void submit(const Renderable2D* renderable) override;
		void render() override;
	};

} }