#include "ddpch.h"
#include "orthographicCamera.h"

namespace daedalusCore { namespace graphics {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		: m_projectionMatrix(maths::mat4::orthographic(left, right, bottom, top, zNear, zFar)), m_viewMatrix(1)
	{
		m_ProjectionviewMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		m_projectionMatrix = (maths::mat4::orthographic(left, right, bottom, top, zNear, zFar));
		m_ProjectionviewMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::recalcViewMatrix()
	{
		m_viewMatrix = maths::mat4::invert(maths::mat4::translate(m_position) * maths::mat4::rotate(m_zRotation, maths::vec3(0, 0, 1)));
		m_ProjectionviewMatrix = m_projectionMatrix * m_viewMatrix;
	}

} }