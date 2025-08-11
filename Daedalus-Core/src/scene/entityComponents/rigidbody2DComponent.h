#pragma once

namespace daedalus::scene {

	struct Rigidbody2DComponent
	{
		enum class BodyType { Static = 0, Dynamic, Kinematic };
		BodyType type = BodyType::Static;
		bool fixedRotation = false;

		// Should have a non-zero density, otherwise will have "strange" behavior.
		float desity = 1.0f;
		float friction = 0.5f;
		float restitution = 0.0f;

		Rigidbody2DComponent() = default;
		Rigidbody2DComponent(const Rigidbody2DComponent&) = default;
	};

}