#pragma once

#include "maths/mat4.h"
#include "maths/vec4.h"
#include "sceneCamera.h"
#include "scriptableEntity.h"
#include "application/time/deltaTime.h"

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
		maths::Vec3 Position = { 0.0f };
		maths::Vec3 Rotation = { 0.0f };
		maths::Vec3 Scale = { 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const maths::Vec3& position)
			: Position(position)
		{ }

		maths::Mat4 getTransform() const
		{
			maths::Mat4 rotationMat = maths::Mat4::rotate(Rotation.x, { 1, 0, 0 })
				* maths::Mat4::rotate(Rotation.y, { 0, 1, 0 })
				* maths::Mat4::rotate(Rotation.z, { 0, 0, 1 });

			return maths::Mat4::translate(Position)
				* rotationMat
				* maths::Mat4::scale(Scale);
		}
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
		maths::Vec4 Colour = {1.0f};

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const maths::Vec4& colour)
			: Colour(colour)
		{ }
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;
		bool ScriptBound = false;

		ScriptableEntity*(*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
			ScriptBound = true;
		}
	};

}
