#pragma once

#include "../../core.h"
#include "../../maths/vec2.h"

namespace daedalus::application {

	class Input
	{
	public:
		/*static bool getKey(int keycode);*/
		static bool getKeyUp(int keycode);
		static bool getKeyDown(int keycode);

		static bool getMouseButton(int button);
		static maths::vec2 getMousePosition();
		static float getMouseX();
		static float getMouseY();
	};

}