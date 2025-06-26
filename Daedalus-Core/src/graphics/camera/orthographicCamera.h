#pragma once

#include "maths/maths.h"

namespace daedalus { namespace graphics {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

		void setProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
		
		void setPosition(const maths::Vec3& position) { m_position = position; recalcViewMatrix(); }
		void setRotation(float zRotation) { m_zRotation = zRotation; recalcViewMatrix(); }
		const maths::Vec3& getPosition() const { return m_position; }
		const float& getRotation() const { return m_zRotation; }

		const maths::Mat4& getProjectionMatrix() const { return m_projectionMatrix; }
		const maths::Mat4& getViewMatrix() const { return m_viewMatrix; }
		const maths::Mat4& getProjectViewMatrix() const { return m_ProjectionviewMatrix; }


	private:
		void recalcViewMatrix();

	private:
		maths::Mat4 m_projectionMatrix;
		maths::Mat4 m_viewMatrix;
		maths::Mat4 m_ProjectionviewMatrix;

		maths::Vec3 m_position;
		float m_zRotation = 0.0f;
	};

} }