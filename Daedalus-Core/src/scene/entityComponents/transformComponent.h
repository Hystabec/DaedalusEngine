#pragma once

#include "maths/mat4.h"
#include "maths/vec4.h"
#include "maths/quaternion.h"

namespace daedalus::scene {

	struct TransformComponent
	{
		maths::Vec3 position = { 0.0f };
		maths::Vec3 rotation = { 0.0f };
		maths::Vec3 scale = { 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const maths::Vec3& position)
			: position(position)
		{
		}

		maths::Mat4 getTransform() const
		{
			maths::Mat4 rotationMat = maths::experimental::quaterion_to_mat4(maths::experimental::Quaternion(rotation));

			return maths::Mat4::translate(position)
				* rotationMat
				* maths::Mat4::scale(scale);
		}
	};

}