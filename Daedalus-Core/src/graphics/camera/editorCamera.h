#pragma once

#include "camera.h"
#include "application/time/deltaTime.h"

#include "events/event.h"
#include "events/mouseEvent.h"

#include "maths/mat4.h"
#include "maths/vec2.h"
#include "maths/vec3.h"
#include "maths/quaternion.h"

namespace daedalus::graphics {

	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float FOV, float aspectRatio, float nearClip, float farClip);

		void update(const application::DeltaTime& dt);
		void onEvent(event::Event& e);

		inline float getDistance() const { return m_distance; }
		inline void setDistance(float distance) { m_distance = distance; }

		inline void setViewportSize(float width, float height) { m_viewportWidth = width; m_viewportHeight = height; updateProjection(); }

		const maths::Mat4& getViewMatrix() const { return m_viewMatrix; }
		maths::Mat4 getViewProjection() const { return m_projection * m_viewMatrix; }

		maths::Vec3 getUpDirection() const;
		maths::Vec3 getRightDirection() const;
		maths::Vec3 getFowardDirection() const;
		const maths::Vec3& getPosition() const { return m_position; }
		maths::experimental::Quaternion getOrientation() const; 
		float getPitch() const { return m_pitch; }
		float getYaw() const { return m_yaw; }

	private:
		void updateProjection();
		void updateView();

		bool onMouseScroll(event::MouseScrolledEvent& e);

		void mousePan(const maths::Vec2& delta);
		void mousePanZ(const maths::Vec2& delta);
		void mouseRotate(const maths::Vec2& delta);
		void mouseZoom(float delta);

		maths::Vec3 calculatePosition() const;
		std::pair<float, float> panSpeed() const;
		float rotationSpeed() const;
		float zoomSpeed() const;

	private:
		float m_FOV = 45.0f, m_aspectRatio = 1.778f, m_nearClip = 0.1f, m_farClip = 1000.0f;

		maths::Mat4 m_viewMatrix;
		maths::Vec3 m_position = { 0.0f };
		maths::Vec3 m_focalPoint = { 0.0f };

		maths::Vec2 m_initialMousePosition = { 0.0f };

		float m_distance = 10.0f;
		float m_pitch = 0.0f, m_yaw = 0.0f;

		float m_viewportWidth = 1280, m_viewportHeight = 720;
	};

}