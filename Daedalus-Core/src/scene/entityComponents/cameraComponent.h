#pragma once

#include "../sceneCamera.h"

namespace daedalus::scene {

	struct CameraComponent
	{
		SceneCamera Camera;
		bool Primary = true; // TO DO: Think about moving to scene
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}