#include "gameCamera.h"
#include "vectorUtils.h"

namespace jumper
{

	GameCamera::GameCamera(float aspectRatio, float minYLevel, bool useCameraTrailing, float trailSpeed, float maxTrailDistance)
		: m_aspectRatio(aspectRatio), m_bounds({ -aspectRatio, aspectRatio, -1, 1 }), m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top),
		m_minYLevel(minYLevel), m_useCameraTrailing(useCameraTrailing), m_trailSpeed(trailSpeed), m_maxTrailDistance(maxTrailDistance)
	{
	}

	void GameCamera::update(const daedalusCore::application::DeltaTime& dt, const JumperMan& character)
	{
		if (m_useCameraTrailing)
		{
			auto mag = vectorUtils::magnitudeVector3(character.getPosition() - this->m_cameraPosition);
			if (mag > m_maxTrailDistance)
				m_cameraPosition += (vectorUtils::normalizeVector3(character.getPosition() - this->m_cameraPosition)) * (float)dt;
			else
				m_cameraPosition += ((vectorUtils::normalizeVector3(character.getPosition() - this->m_cameraPosition)) * m_trailSpeed) * mag * (float)dt;;
		}
		else
			m_cameraPosition = character.getPosition();

		if (m_cameraPosition.y < m_minYLevel)
			m_cameraPosition.y = m_minYLevel;

		m_camera.setPosition(m_cameraPosition);
	}

	void GameCamera::onEvent(daedalusCore::event::Event& e)
	{
		daedalusCore::event::EventDispatcher dispatcher(e);
		dispatcher.dispatch<daedalusCore::event::WindowResizedEvent>(DD_BIND_EVENT_FUN(GameCamera::onWindowResize));
	}

	bool GameCamera::onWindowResize(daedalusCore::event::WindowResizedEvent& e)
	{
		m_aspectRatio = (float)e.getWidth() / (float)e.getHeight();
		m_camera.setProjection(-m_aspectRatio, m_aspectRatio, -1, 1);

		m_bounds.left = -m_aspectRatio;
		m_bounds.right = m_aspectRatio;

		return false;
	}

}