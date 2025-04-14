#pragma once

#include "maths/maths.h"

namespace daedalusCore { namespace graphics {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

		void setProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
		
		void setPosition(const maths::vec3& position) { m_position = position; recalcViewMatrix(); }
		void setRotation(float zRotation) { m_zRotation = zRotation; recalcViewMatrix(); }
		const maths::vec3& getPosition() const { return m_position; }
		const float& getRotation() const { return m_zRotation; }

		const maths::mat4& getProjectionMatrix() const { return m_projectionMatrix; }
		const maths::mat4& getViewMatrix() const { return m_viewMatrix; }
		const maths::mat4& getProjectViewMatrix() const { return m_ProjectionviewMatrix; }


	private:
		void recalcViewMatrix();

	private:
		maths::mat4 m_projectionMatrix;
		maths::mat4 m_viewMatrix;
		maths::mat4 m_ProjectionviewMatrix;

		maths::vec3 m_position;
		float m_zRotation = 0.0f;
	};

} }