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

		inline static bool getMouseButton(int button);
		inline static maths::vec2 getMousePosition();
		inline static float getMouseX();
		inline static float getMouseY();
	};

}