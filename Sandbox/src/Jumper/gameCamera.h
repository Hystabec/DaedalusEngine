#pragma once

#include <Daedalus.h>

#include "jumperMan.h"

namespace jumper
{

	class GameCamera
	{
	public:
		GameCamera(float aspectRatio, float minYLevel, bool useCameraTrailing = false, float trailSpeed = 1.0f, float maxTrailDistance = 0.5f);

		void update(const daedalusCore::application::DeltaTime& dt, const JumperMan& character);

		void onEvent(daedalusCore::event::Event& e);

		inline const daedalusCore::graphics::OrthographicCamera& getCamera() const { return m_camera; }

		inline const daedalusCore::graphics::OrthgraphicCameraBounds& getBounds() const { return m_bounds; }
		inline const daedalusCore::maths::vec3& getPosition() const { return m_cameraPosition; }

	private:
		bool onWindowResize(daedalusCore::event::WindowResizedEvent& e);

	private:
		float m_aspectRatio;
		daedalusCore::graphics::OrthgraphicCameraBounds m_bounds;
		daedalusCore::graphics::OrthographicCamera m_camera;

		daedalusCore::maths::vec3 m_cameraPosition = { 0.0f,0.0f,0.0f };

		float m_minYLevel;
		
		bool m_useCameraTrailing;
		float m_trailSpeed;
		float m_maxTrailDistance;
	};

}