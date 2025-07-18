#pragma once
#include "inputCodes.h"

namespace daedalus::application::utils
{
	// currently a direct conversion
	InputCode glfw_keycode_to_DD_keycode(int key)
	{
		return (InputCode)key;
	}
}