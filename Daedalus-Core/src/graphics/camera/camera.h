#pragma once
#include "maths/mat4.h"

namespace daedalus::graphics {

	class Camera
	{
	public:
		Camera(const maths::Mat4& projection)
			: m_projection(projection)
		{
		}

		const maths::Mat4& getProjection() const { return m_projection; }

		// TO DO:
		// void setPerspective
		// void setOtho
	private:
		maths::Mat4 m_projection;
	};

}