#pragma once

#include "maths/mat4.h"
#include "maths/vec4.h"

// if this file gets too big, should seperate into individual files.

namespace daedalus::scene {

	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag)
		{
		}
	};
	
	struct TransformComponent
	{
		maths::mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const maths::mat4& transform)
			: Transform(transform)
		{ }

		operator maths::mat4& () { return Transform; }
		operator const maths::mat4& () const { return Transform; }
	};

	struct SpriteRendererComponent
	{
		maths::vec4 Colour;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const maths::vec4& colour)
			: Colour(colour)
		{ }
	};

}
