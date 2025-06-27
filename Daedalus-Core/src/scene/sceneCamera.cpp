#include "ddpch.h"
#include "sceneCamera.h"

namespace daedalus::scene {

	SceneCamera::SceneCamera()
	{
		recalculateProjection();
	}

	void SceneCamera::setOrthographic(float size, float nearClip, float farClip)
	{
		m_orthoSize = size;
		m_orthoNear = nearClip;
		m_orthoFar = farClip;

		recalculateProjection();
	}

	void SceneCamera::setViewportSize(uint32_t width, uint32_t height)
	{
		m_aspectRatio = (float)width / (float)height;

		recalculateProjection();
	}

	void SceneCamera::recalculateProjection()
	{
		float orthoLeft = -m_orthoSize * m_aspectRatio * 0.5f;
		float orthoRight = m_orthoSize * m_aspectRatio * 0.5f;
		float orthoBottom = -m_orthoSize * 0.5f;
		float orthoTop = m_orthoSize * 0.5f;

		m_projection = maths::Mat4::orthographic(orthoLeft, orthoRight, orthoBottom, orthoTop, m_orthoNear, m_orthoFar);
	}

}