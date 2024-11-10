#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>

namespace daedalusCore { namespace graphics {

	class Window
	{
	private:
		const char* m_title = "";
		int m_width = 0, m_height = 0;
		GLFWwindow* m_window;
		bool m_closed = false;

	public:
		Window(const char* title, int width, int height);
		~Window();
		void clear() const;
		void update();
		bool closed() const;

		//inline int getWidth() const { return m_width; };
		//inline int getHeight() const { return m_height; };
	};

} }