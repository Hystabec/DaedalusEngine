#pragma once

#include "maths/mat4.h"
#include "maths/vec4.h"
#include "sceneCamera.h"

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
		maths::Mat4 Transform;

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const maths::Mat4& transform)
			: Transform(transform)
		{ }

		operator maths::Mat4& () { return Transform; }
		operator const maths::Mat4& () const { return Transform; }
	};

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TO DO: Think about moving to scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct SpriteRendererComponent
	{
		maths::Vec4 Colour;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const maths::Vec4& colour)
			: Colour(colour)
		{ }
	};

}
