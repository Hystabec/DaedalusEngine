#include "ddpch.h"
#include "orthographicCameraController.h"

#include "application/input/input.h"
#include "application/input/inputCodes.h"
#include "application/applicationCore.h"

namespace daedalus { namespace graphics {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool allowRotation)
		: m_aspectRatio(aspectRatio), m_bounds({ -aspectRatio * m_zoomLevel, aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel }), m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top), m_useRotation(allowRotation)
	{
	}

	void OrthographicCameraController::update(const application::DeltaTime& dt)
	{
		DD_PROFILE_FUNCTION();

		if (application::Input::getKeyDown(application::InputCode::Key_W))
			m_cameraPosition.y += m_cameraTranslationSpeed * dt;
		if (application::Input::getKeyDown(application::InputCode::Key_S))
			m_cameraPosition.y -= m_cameraTranslationSpeed * dt;

		if (application::Input::getKeyDown(application::InputCode::Key_A))
			m_cameraPosition.x -= m_cameraTranslationSpeed * dt;
		if (application::Input::getKeyDown(application::InputCode::Key_D))
			m_cameraPosition.x += m_cameraTranslationSpeed * dt;

		m_camera.setPosition(m_cameraPosition);

		if (m_useRotation)
		{
			if (application::Input::getKeyDown(application::InputCode::Key_Q))
				m_cameraRotation += m_cameraRotationSpeed * dt;
			if (application::Input::getKeyDown(application::InputCode::Key_E))
				m_cameraRotation -= m_cameraRotationSpeed * dt;

			m_camera.setRotation(m_cameraRotation);
		}
	}

	void OrthographicCameraController::setZoomLevel(float level)
	{
		m_zoomLevel = level;

		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.setProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);

		m_cameraTranslationSpeed = m_zoomLevel;
	}

	maths::Vec2 OrthographicCameraController::mouseToWorldPosition(maths::Vec2 mousePos) const
	{
		auto width = Application::get().getWindow()->getWidth();
		auto height = Application::get().getWindow()->getHeight();

		float x = (mousePos.x / width) * m_bounds.getWidth() - m_bounds.getWidth() * 0.5f;
		float y = m_bounds.getHeight() * 0.5f - (mousePos.y / height) * m_bounds.getHeight();

		return {x + m_cameraPosition.x, y + m_cameraPosition.y};
	}

	void OrthographicCameraController::onEvent(event::Event& e)
	{
		DD_PROFILE_FUNCTION();

		event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<event::MouseScrolledEvent>(DD_BIND_EVENT_FUN(OrthographicCameraController::onMouseScrolled));
		dispatcher.dispatch<event::WindowResizedEvent>(DD_BIND_EVENT_FUN(OrthographicCameraController::onWindowResize));
	}

	void OrthographicCameraController::onResize(float width, float height)
	{
		m_aspectRatio = width / height;
		m_camera.setProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel);
		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
	}

	bool OrthographicCameraController::onMouseScrolled(event::MouseScrolledEvent& e)
	{
		DD_PROFILE_FUNCTION();

		m_zoomLevel -= e.getY() * 0.25f;
		m_zoomLevel = ((m_zoomLevel) > (0.25f)) ? (m_zoomLevel) : (0.25f);

		m_bounds = { -m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel };
		m_camera.setProjection(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);

		m_cameraTranslationSpeed = m_zoomLevel;

		return false;
	}

	bool OrthographicCameraController::onWindowResize(event::WindowResizedEvent& e)
	{
		DD_PROFILE_FUNCTION();

		onResize((float)e.getWidth(), (float)e.getHeight());

		return false;
	}


} }


