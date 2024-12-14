#pragma once

#include "ddpch.h"
#include "events/event.h"

namespace daedalusCore { namespace application {

	struct WindowProperties
	{
		std::string Title;
		unsigned int Width, Height;
		bool VSync;

		WindowProperties(const std::string& title = "Daedalus", unsigned int width = 960, unsigned int height = 540, bool vsync = false)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{
		}
	};

	class DD_API Window
	{
	public:
		using EventCallbackFn = std::function<void(daedalusCore::event::Event&)>;

		virtual ~Window() {}
		
		virtual void Update() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProperties& props = WindowProperties());
	};

} }