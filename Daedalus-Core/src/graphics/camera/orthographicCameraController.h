#pragma once

#include "orthographicCamera.h"
#include "application/time/deltaTime.h"

#include "events/mouseEvent.h"
#include "events/windowEvent.h"

namespace daedalusCore { namespace graphics {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool allowRotation = false);

		void update(const application::DeltaTime& dt);
		void onEvent(event::Event& e);

		OrthographicCamera& getCamera() { return m_camera; }
		const OrthographicCamera& getCamera() const { return m_camera; }

		void setPosition(const maths::vec3& position) { m_cameraPosition = position; m_camera.setPosition(m_cameraPosition); }
		void setRotation(float zRotation) { m_cameraRotation = zRotation; m_camera.setRotation(m_cameraRotation); }
		const maths::vec3& getPosition() const { return m_cameraPosition; }
		const float& getRotation() const { return m_cameraRotation; }

		const float& getZoomLevel() const { return m_zoomLevel; }
		void setZoomLevel(float level) { m_zoomLevel = level; }

	private:
		bool onMouseScrolled(event::MouseScrolledEvent& e);
		bool onWindowResize(event::WindowResizedEvent& e);

	private:
		float m_aspectRatio;
		float m_zoomLevel = 1.0f;

		OrthographicCamera m_camera;

		bool m_useRotation;

		maths::vec3 m_cameraPosition = { 0.0f,0.0f,0.0f };
		float m_cameraRotation = 0.0f;
		float m_cameraTranslationSpeed = 5.0f;
		float m_cameraRotationSpeed = 90.0f;
	};

} }