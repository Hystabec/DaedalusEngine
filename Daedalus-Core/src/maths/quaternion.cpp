#include "ddpch.h"
#include "quaternion.h"

namespace daedalus::maths::experimental {

	Quaternion::Quaternion(const Vec3& euler)
	{
		Vec3 c = { cos(euler.x * 0.5f), cos(euler.y * 0.5f), cos(euler.z * 0.5f) };
		Vec3 s = { sin(euler.x * 0.5f), sin(euler.y * 0.5f), sin(euler.z * 0.5f) };

		this->w = c.x * c.y * c.z + s.x * s.y * s.z;
		this->x = s.x * c.y * c.z - c.x * s.y * s.z;
		this->y = c.x * s.y * c.z + s.x * c.y * s.z;
		this->z = c.x * c.y * s.z - s.x * s.y * c.z;
	}

	Mat4 quaterion_to_mat4(const Quaternion& quat)
	{
		Mat4 result(1.0f);

		float qxx = quat.x * quat.x;
		float qyy = quat.y * quat.y;
		float qzz = quat.z * quat.z;

		float qxz = quat.x * quat.z;
		float qxy = quat.x * quat.y;
		float qyz = quat.y * quat.z;
		float qwx = quat.w * quat.x;
		float qwy = quat.w * quat.y;
		float qwz = quat.w * quat.z;

		result.columns[0][0] = 1.0f - 2.0f * (qyy + qzz);
		result.columns[0][1] = 2.0f * (qxy + qwz);
		result.columns[0][2] = 2.0f * (qxz - qwy);

		result.columns[1][0] = 2.0f * (qxy - qwz);
		result.columns[1][1] = 1.0f - 2.0f * (qxx + qzz);
		result.columns[1][2] = 2.0f * (qyz + qwx);

		result.columns[2][0] = 2.0f * (qxz + qwy);
		result.columns[2][1] = 2.0f * (qyz - qwx);
		result.columns[2][2] = 1.0f - 2.0f * (qxx + qyy);

		return result;
	}

	Vec3 rotate_vec3_by_quaternion(const Quaternion& quat, const Vec3& vec)
	{
		const Vec3 quatVec(quat.x, quat.y, quat.z);
		const Vec3 uv(Vec3::cross(quatVec, vec));
		const Vec3 uuv(Vec3::cross(quatVec, uv));

		return vec + (((uv * quat.w) + uuv) * 2.0f);
	}

}