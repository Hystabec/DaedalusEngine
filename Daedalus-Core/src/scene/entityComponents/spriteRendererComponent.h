#pragma once

#include "maths/vec4.h"
#include "graphics/rendering/material2D.h"

namespace daedalus::scene {

	struct SpriteRendererComponent
	{
		maths::Vec4 colour = { 1.0f };
		graphics::Material2D material;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const maths::Vec4& colour)
			: colour(colour)
		{
		}
	};

}