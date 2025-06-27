#pragma once
#include "maths/mat4.h"

namespace daedalus::graphics {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const maths::Mat4& projection)
			: m_projection(projection)
		{
		}

		virtual ~Camera() = default;

		const maths::Mat4& getProjection() const { return m_projection; }

		// TO DO:
		// void setPerspective
		// void setOtho
	protected:
		maths::Mat4 m_projection = maths::Mat4::identity();
	};

}