#pragma once

#include <Daedalus.h>

#include "jumperMan.h"

namespace jumper
{

	class GameCamera
	{
	public:
		GameCamera(float aspectRatio, float minYLevel, bool useCameraTrailing = false, float trailSpeed = 0.75f, float maxTrailDistance = 0.25f);

		void update(const daedalusCore::application::DeltaTime& dt, const JumperMan& character);

		inline const daedalusCore::graphics::OrthographicCamera& getCamera() const { return m_camera; }

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