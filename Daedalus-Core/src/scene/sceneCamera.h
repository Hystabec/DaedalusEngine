#pragma once
#include "graphics/camera/camera.h"
#include "maths/mathsHelpers.h"

namespace daedalus::scene {

	class SceneCamera : public graphics::Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Othographic = 1 };
	public:
		SceneCamera();
		~SceneCamera() override = default;

		void setOrthographic(float size, float nearClip, float farClip);
		void setPerspective(float verticalFOV, float nearClip, float farClip);

		void setViewportSize(uint32_t width, uint32_t height);

		float getPerspectiveVerticalFOV() const { return m_perspecFOV; }
		void setPerspectiveVerticalFOV(float verticalFOV) { m_perspecFOV = verticalFOV; recalculateProjection(); }
		float getPerspectiveNearClip() const { return m_perspecNear; }
		float getPerspectiveFarClip() const { return m_perspecFar; }
		void setPerspectiveNearClip(float Near) { m_perspecNear = Near; recalculateProjection(); }
		void setPerspectiveFarClip(float Far) { m_perspecFar = Far; recalculateProjection(); }

		float getOrthographicSize() const { return m_orthoSize; }
		void setOrthographicSize(float size) { m_orthoSize = size; recalculateProjection(); }
		float getOrthographicNearClip() const { return m_orthoNear; }
		float getOrthographicFarClip() const { return m_orthoFar; }
		void setOrthographicNearClip(float Near) { m_orthoNear = Near; recalculateProjection(); }
		void setOrthographicFarClip(float Far) { m_orthoFar = Far; recalculateProjection(); }

		ProjectionType getProjectionType() const { return m_projectionType; }
		void setProjectionType(ProjectionType type) { m_projectionType = type; recalculateProjection(); }
	private:
		void recalculateProjection();
	private:
		ProjectionType m_projectionType = ProjectionType::Othographic;

		float m_perspecFOV = maths::degrees_to_radians(45.0f);
		float m_perspecNear = 0.01f, m_perspecFar = 1000.0f;

		float m_orthoSize = 10.0f;
		float m_orthoNear = -1.0f, m_orthoFar = 1.0f;

		float m_aspectRatio = 0.0f;
	};

}