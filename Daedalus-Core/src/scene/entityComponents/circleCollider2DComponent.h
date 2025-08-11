#pragma once

#include "maths/vec2.h"

namespace daedalus::scene {

	struct CircleCollider2DComponent
	{
		maths::Vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

}