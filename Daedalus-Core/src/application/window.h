#pragma once

#include "ddpch.h"
#include "events/event.h"

namespace daedalus { namespace application {

	struct WindowProperties
	{
		std::string Title;
		uint32_t Width, Height;
		bool VSync;

		WindowProperties(const std::string& title = "Daedalus", uint32_t width = 960, uint32_t height = 540, bool vsync = false)
			: Title(title), Width(width), Height(height), VSync(vsync)
		{
		}
	};

	class  Window
	{
	public:
		using EventCallbackFn = std::function<void(daedalus::event::Event&)>;

		virtual ~Window() {}
		
		virtual void update() = 0;

		virtual uint32_t getWidth() const = 0;
		virtual uint32_t getHeight() const = 0;

		virtual void setEventCallback(const EventCallbackFn& callback) = 0;
		virtual void setVSync(bool enabled) = 0;
		virtual bool isVSync() const = 0;

		virtual void setWindowName(const std::string& name) = 0;
		virtual void setWindowIcon(const std::filesystem::path& path) = 0;

		virtual void* getNativeWindow() const = 0;

		static ScopedPtr<Window> Create(const WindowProperties& props = WindowProperties());
	};

} }