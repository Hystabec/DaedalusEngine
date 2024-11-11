#pragma once

#include <iostream>
#include <GL/glew.h>
#include <glfw3.h>

namespace daedalusCore { namespace graphics {

#define MAX_KEYS	1024
#define MAX_BUTTONS 32

	class Window
	{
	private:
		const char* m_title = "";
		int m_width = 0, m_height = 0;
		GLFWwindow* m_window;
		bool m_closed = false;

		bool m_keys[MAX_KEYS];
		bool m_MouseButtons[MAX_BUTTONS];
		double m_mouseX, m_mouseY;
	public:
		Window(const char* title, int width, int height);
		~Window();
		void clear() const;
		void update();
		bool closed() const;

		//inline int getWidth() const { return m_width; };
		//inline int getHeight() const { return m_height; };

		bool getKey(unsigned int keycode) const;
		bool getMouse(unsigned int buttonCode) const;
		void getMousePosition(double& xPos, double& yPos) const;
	private:
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	};

} }