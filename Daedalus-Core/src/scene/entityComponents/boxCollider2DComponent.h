#pragma once

#include "maths/vec2.h"

namespace daedalus::scene {

	struct BoxCollider2DComponent
	{
		maths::Vec2 offset = { 0.0f, 0.0f };
		maths::Vec2 size = { 0.5f, 0.5f };

		// This should probably be on the rigidbody, and in a seperate physics material struct

		// Should have a non-zero density, otherwise will have "strange" behavior.
		float desity = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;
		//float restitutionThreshold = 0.5f; // This is not implement in the serializer if i re add it

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

}