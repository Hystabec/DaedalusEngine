#pragma once

#include "../../core.h"
#include "../../maths/vec2.h"
#include "inputCodes.h"

namespace daedalus::application {

	class Input
	{
	public:
		/*static bool getKey(int keycode);*/
		static bool getKeyUp(InputCode keycode);
		static bool getKeyDown(InputCode keycode);

		static bool getMouseButton(InputCode button);
		static maths::Vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};

}