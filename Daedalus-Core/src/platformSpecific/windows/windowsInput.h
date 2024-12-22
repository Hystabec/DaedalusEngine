#pragma once

#include "application/Input.h"

namespace daedalusCore { namespace application {

	class windowsInput : public Input
	{
	protected:
		//bool GetKeyBase(int keycode) override;
		bool GetKeyUpBase(int keycode) override;
		bool GetKeyDownBase(int keycode) override;

		bool GetMouseButtonBase(int button) override;
		std::pair<float, float> GetMousePositionBase() override;
		float GetMouseXBase() override;
		float GetMouseYBase() override;
	};

} }