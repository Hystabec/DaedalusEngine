#pragma once

#include "maths/vec2.h"

namespace daedalus::scene {

	struct BoxCollider2DComponent
	{
		maths::Vec2 offset = { 0.0f, 0.0f };
		maths::Vec2 size = { 0.5f, 0.5f };

		BoxCollider2DComponent() = default;
		BoxCollider2DComponent(const BoxCollider2DComponent&) = default;
	};

}