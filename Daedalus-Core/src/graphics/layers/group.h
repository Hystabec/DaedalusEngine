#pragma once
	
#include "graphics/renderable2D.h"

namespace daedalusCore { namespace graphics {

	class Group : public Renderable2D
	{
	public:
		Group(const maths::mat4& transform);
		~Group();

		void add(Renderable2D* renderable);
		void submit(Renderer2D* renderer) const override;

	private:
		std::vector<Renderable2D*> m_renderables;
		maths::mat4 m_transformationMatrix;
	};

} }