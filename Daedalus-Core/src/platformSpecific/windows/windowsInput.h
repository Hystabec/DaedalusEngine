#pragma once

#include "application/input/Input.h"

namespace daedalusCore { namespace application {

	class windowsInput : public Input
	{
	protected:
		//bool GetKeyBase(int keycode) override;
		bool getKeyUpBase(int keycode) override;
		bool getKeyDownBase(int keycode) override;

		bool getMouseButtonBase(int button) override;
		maths::vec2 getMousePositionBase() override;
		float getMouseXBase() override;
		float getMouseYBase() override;
	};

} }