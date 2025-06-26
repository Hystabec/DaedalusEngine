#include "ddpch.h"
#include "orthographicCamera.h"

namespace daedalus { namespace graphics {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, float zNear, float zFar)
		: m_projectionMatrix(maths::Mat4::orthographic(left, right, bottom, top, zNear, zFar)), m_viewMatrix(1)
	{
		DD_PROFILE_FUNCTION();

		m_ProjectionviewMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::setProjection(float left, float right, float bottom, float top, float zNear, float zFar)
	{
		DD_PROFILE_FUNCTION();

		m_projectionMatrix = (maths::Mat4::orthographic(left, right, bottom, top, zNear, zFar));
		m_ProjectionviewMatrix = m_projectionMatrix * m_viewMatrix;
	}

	void OrthographicCamera::recalcViewMatrix()
	{
		DD_PROFILE_FUNCTION();

		m_viewMatrix = maths::Mat4::invert(maths::Mat4::translate(m_position) * maths::Mat4::rotate(m_zRotation, maths::Vec3(0, 0, 1)));
		m_ProjectionviewMatrix = m_projectionMatrix * m_viewMatrix;
	}

} }