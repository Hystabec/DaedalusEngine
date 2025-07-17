#pragma once

#include "maths/vec4.h"

namespace daedalus::scene {

	struct SpriteRendererComponent
	{
		maths::Vec4 Colour = { 1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const maths::Vec4& colour)
			: Colour(colour)
		{
		}
	};

}