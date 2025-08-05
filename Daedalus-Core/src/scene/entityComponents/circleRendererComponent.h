#pragma once

#include "maths/vec4.h"

namespace daedalus::scene {

	struct CircleRendererComponent
	{
		maths::Vec4 colour = { 1.0f };
		float thickness = 1.0f;
		float fade = 0.0f;

		CircleRendererComponent() = default;
		CircleRendererComponent(const CircleRendererComponent&) = default;
	};

}