#pragma once
#include "maths/mat4.h"

namespace daedalus::graphics {

	class Camera
	{
	public:
		Camera(const maths::mat4& projection)
			: m_projection(projection)
		{
		}

		const maths::mat4& getProjection() const { return m_projection; }

		// TO DO:
		// void setPerspective
		// void setOtho
	private:
		maths::mat4 m_projection;
	};

}