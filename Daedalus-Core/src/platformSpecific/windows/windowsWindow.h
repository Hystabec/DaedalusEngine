#pragma once

#include "graphics/window.h"

struct GLFWwindow;

namespace daedalusCore { namespace graphics {

	class WindowsWindow : public Window
	{
	private:
		GLFWwindow* m_window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool Vsync;

			EventCallbackFn EventCallBack;
		};

		WindowData m_data;

	private:
		virtual void Init(const WindowProperties& props);
		virtual void Shutdown();

	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		void Update() override;

		inline unsigned int GetWidth() const override { return m_data.Width; }
		inline unsigned int GetHeight() const override { return m_data.Height; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_data.EventCallBack = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
	};

} }