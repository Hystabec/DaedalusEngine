#pragma once

#include "../core.h"

namespace daedalusCore { namespace application {

	class DD_API Input
	{
	private:
		static Input* m_instance;

	protected:
		virtual bool GetKeyBase(int keycode) = 0;
		virtual bool GetKeyUpBase(int keycode) = 0;
		virtual bool GetKeyDownBase(int keycode) = 0;

		virtual bool GetMouseButtonBase(int button) = 0;
		virtual std::pair<float, float> GetMousePositionBase() = 0;
		virtual float GetMouseXBase() = 0;
		virtual float GetMouseYBase() = 0;

	public:
		inline static bool GetKey(int keycode) { return m_instance->GetKeyBase(keycode); }
		static bool GetKeyUp(int keycode) { return m_instance->GetKeyUpBase(keycode); }
		static bool GetKeyDown(int keycode) { return m_instance->GetKeyDownBase(keycode); }

		inline static bool GetMouseButton(int button) { return m_instance->GetMouseButtonBase(button); }
		inline static std::pair<float,float> GetMousePosition() { return m_instance->GetMousePositionBase(); }
		inline static float GetMouseX() { return m_instance->GetMouseXBase(); }
		inline static float GetMouseY() { return m_instance->GetMouseYBase(); }
	};

} }