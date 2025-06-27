#pragma once
#include "graphics/camera/camera.h"

namespace daedalus::scene {

	class SceneCamera : public graphics::Camera
	{
	public:
		SceneCamera();
		~SceneCamera() override = default;

		void setOrthographic(float size, float nearClip, float farClip);
		void setViewportSize(uint32_t width, uint32_t height);

		float getOrthographicSize() const { return m_orthoSize; }
		void setOrthographicSize(float size) { m_orthoSize = size; recalculateProjection(); }

		float getOrthographicNear() const { return m_orthoNear; }
		void setOrthographicNear(float Near) { m_orthoNear = Near; recalculateProjection(); }

		float getOrthographicFar() const { return m_orthoFar; }
		void setOrthographicFar(float Far) { m_orthoFar = Far; recalculateProjection(); }
	private:
		void recalculateProjection();
	private:
		float m_orthoSize = 10.0f;
		float m_orthoNear = -1.0f, m_orthoFar = 1.0f;

		float m_aspectRatio = 0.0f;
	};

}