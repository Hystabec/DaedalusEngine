#pragma once

#include "../sceneCamera.h"

namespace daedalus::scene {

	struct CameraComponent
	{
		SceneCamera camera;
		bool primary = true; // TO DO: Think about moving to scene
		bool fixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

}