#pragma once

#include "application/window.h"

struct GLFWwindow;

namespace daedalusCore {
	namespace graphics {
		class RenderingContext;
	}
	namespace application {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProperties& props);
		virtual ~WindowsWindow();

		void update() override;

		inline unsigned int getWidth() const override { return m_data.Width; }
		inline unsigned int getHeight() const override { return m_data.Height; }

		inline void setEventCallback(const EventCallbackFn& callback) override { m_data.EventCallBack = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		inline void* getNativeWindow() const override { return m_window; }

	private:
		virtual void init(const WindowProperties& props);
		virtual void shutdown();

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

		graphics::RenderingContext* m_renderingContext;
	};

} }