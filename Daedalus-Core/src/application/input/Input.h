#pragma once

#include "../../core.h"
#include "../../maths/vec2.h"

namespace daedalus { namespace application {

	class Input
	{
	public:
		//inline static bool GetKey(int keycode) { return m_instance->GetKeyBase(keycode); }
		static bool getKeyUp(int keycode) { return s_instance->getKeyUpBase(keycode); }
		static bool getKeyDown(int keycode) { return s_instance->getKeyDownBase(keycode); }

		inline static bool getMouseButton(int button) { return s_instance->getMouseButtonBase(button); }
		inline static maths::vec2 getMousePosition() { return s_instance->getMousePositionBase(); }
		inline static float getMouseX() { return s_instance->getMouseXBase(); }
		inline static float getMouseY() { return s_instance->getMouseYBase(); }

	protected:
		//virtual bool GetKeyBase(int keycode) = 0;
		virtual bool getKeyUpBase(int keycode) = 0;
		virtual bool getKeyDownBase(int keycode) = 0;

		virtual bool getMouseButtonBase(int button) = 0;
		virtual maths::vec2 getMousePositionBase() = 0;
		virtual float getMouseXBase() = 0;
		virtual float getMouseYBase() = 0;

	private:
		static Input* s_instance;
	};

} }