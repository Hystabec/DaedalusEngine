#pragma once

#include "application/window.h"

struct GLFWwindow;

namespace daedalus {
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

		inline uint32_t getWidth() const override { return m_data.Width; }
		inline uint32_t getHeight() const override { return m_data.Height; }

		inline void setEventCallback(const EventCallbackFn& callback) override { m_data.EventCallBack = callback; }
		void setVSync(bool enabled) override;
		bool isVSync() const override;

		void setWindowName(const std::string& name) override;
		void setWindowIcon(const std::filesystem::path& path) override;

		inline void* getNativeWindow() const override { return m_window; }

	private:
		virtual void init(const WindowProperties& props);
		virtual void shutdown();

	private:
		GLFWwindow* m_window;

		struct WindowData
		{
			std::string Title;
			uint32_t Width, Height;
			bool Vsync;

			EventCallbackFn EventCallBack;
		};

		WindowData m_data;

		graphics::RenderingContext* m_renderingContext;
	};

} }