#pragma once

#include "vec3.h"
#include "mat4.h"

// This class is used by the transfrom component when creating a projection matrix
// it should stop the singularities/gimbal lock caused by Euler angles

// Should consider converting all angles to be stored as Quaterion instead of Euler

namespace daedalus::maths::experimental{

	struct Quaternion
	{
		float x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;

		Quaternion() = default;
		Quaternion(const Vec3& euler);
	};

	Mat4 quaterion_to_mat4(const Quaternion& quat);

	Vec3 rotate_vec3_by_quaternion(const Quaternion& quat, const Vec3& vec);
}

LOG_CREATE_FORMAT(daedalus::maths::experimental::Quaternion, "x: {} y: {} z: {} w: {}", quat, quat.x, quat.y, quat.z, quat.w);