#include "ddpch.h"
#include "orthographicCameraController.h"

#include "application/input/Input.h"
#include "application/input/inputKeyCodes.h"

namespace daedalusCore { namespace graphics {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool allowRotation)
		: m_aspectRatio(aspectRatio), m_camera(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel), m_useRotation(allowRotation)
	{
	}

	void OrthographicCameraController::update(const application::DeltaTime& dt)
	{
		if (application::Input::getKeyDown(DD_INPUT_KEY_W))
			m_cameraPosition.y += m_cameraTranslationSpeed * dt;
		if (application::Input::getKeyDown(DD_INPUT_KEY_S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * dt;

		if (application::Input::getKeyDown(DD_INPUT_KEY_A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * dt;
		if (application::Input::getKeyDown(DD_INPUT_KEY_D))
			m_cameraPosition.x += m_cameraTranslationSpeed * dt;

		m_camera.setPosition(m_cameraPosition);

		if (m_useRotation)
		{
			if (application::Input::getKeyDown(DD_INPUT_KEY_Q))
				m_cameraRotation += m_cameraRotationSpeed * dt;
			if (application::Input::getKeyDown(DD_INPUT_KEY_E))
				m_cameraRotation -= m_cameraRotationSpeed * dt;

			m_camera.setRotation(m_cameraRotation);
		}
	}

	void OrthographicCameraController::onEvent(event::Event& e)
	{
		event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<event::MouseScrolledEvent>(DD_BIND_EVENT_FUN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<event::WindowResizedEvent>(DD_BIND_EVENT_FUN(OrthographicCameraController::onWindowResize));
	}

	bool OrthographicCameraController::onMouseScrolled(event::MouseScrolledEvent& e)
	{
		m_zoomLevel -= e.getY() * 0.25f;
		m_zoomLevel = ((m_zoomLevel) > (0.25f)) ? (m_zoomLevel) : (0.25f);
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

		m_cameraTranslationSpeed = m_zoomLevel;

		return false;
	}

	bool OrthographicCameraController::onWindowResize(event::WindowResizedEvent& e)
	{
		m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);

		return false;
	}


} }