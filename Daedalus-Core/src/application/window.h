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

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(daedalusCore::event::Event&)>;

		virtual ~Window() {}
		
		virtual void update() = 0;

		virtual unsigned int getWidth() const = 0;
		virtual unsigned int getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void* getNativeWindow() const = 0;

		static uni_ptr<Window> Create(const WindowProperties& props = WindowProperties());
	};

} }