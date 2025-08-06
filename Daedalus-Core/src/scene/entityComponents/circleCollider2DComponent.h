#pragma once

#include "maths/vec2.h"

namespace daedalus::scene {

	struct CircleCollider2DComponent
	{
		maths::Vec2 offset = { 0.0f, 0.0f };
		float radius = 0.5f;

		// This should probably be on the rigidbody, and in a seperate physics material struct
		// Should have a non-zero density, otherwise will have "strange" behavior.
		float desity = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;

		CircleCollider2DComponent() = default;
		CircleCollider2DComponent(const CircleCollider2DComponent&) = default;
	};

}