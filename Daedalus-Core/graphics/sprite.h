#pragma once

#include "renderable2D.h"

namespace daedalusCore { namespace graphics {

	class Sprite : public Renderable2D
	{
	public:
		Sprite(float x, float y, float width, float height, const maths::vec4& colour);
		Sprite(float x, float y, float width, float height, Texture* texture);
	};

} }